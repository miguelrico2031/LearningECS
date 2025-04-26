#include "Game.h"
#include "const.h"
#include "Log.h"
#include "ObjectComponent/Camera.h"
#include "Input.h"


using namespace ObjectComponent;

Game Game::s_singleton;

Scene* Game::createScene()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->unload();
	}
	m_activeScene = std::make_unique<Scene>();
	return m_activeScene.get();
}

void Game::setUp()
{
	bool success = setUpRenderWindow();
	if (!success)
		return;

	Input::initialize(m_window);

}

void Game::runLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		float frameTime = (float)glfwGetTime();
		m_deltaTime = frameTime - m_lastFrameTime;
		m_lastFrameTime = frameTime;
		m_accumulatedTime += m_deltaTime;

		//processInput();

		while (m_accumulatedTime >= TIME::FIXED_DELTA_TIME)
		{
			fixedUpdate();
			m_accumulatedTime -= TIME::FIXED_DELTA_TIME;
		}
		update();
		render();

		glfwPollEvents(); // check and call events
		glfwSwapBuffers(m_window); // swap buffers
	}
}

void Game::dispose()
{
	disposeRenderWindow();

}

bool Game::setUpRenderWindow()
{
	// init and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_DEPTH_BITS, 24);


	// create a window object
	m_window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "LearningECS", NULL, NULL);
	if (m_window == NULL)
	{
		LOG_ERROR("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	// init GLAD (loads all OpenGL function pointers)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to init GLAD");
		return false;
	}

	//configure rendering window size and resize callback
	glViewport(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
	glfwSetFramebufferSizeCallback(m_window, onWindowResize);


	//config OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);       // Enable face culling
	glCullFace(GL_BACK);          // Discards back faces
	glFrontFace(GL_CCW);          // counter clock wise is front

	return true;
}

void Game::disposeRenderWindow()
{
	glfwTerminate();
}

void Game::onWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (Camera* camera = s_singleton.m_activeScene->getActiveCamera())
		camera->setAspectRatio((float)width / (float)height);
}

void Game::update()
{
	if (m_editor) m_editor->update(m_deltaTime);
	m_activeScene->update(m_deltaTime);
	Input::update();
}

void Game::fixedUpdate()
{
	if (m_editor) m_editor->fixedUpdate();
	m_activeScene->fixedUpdate();
}

void Game::render()
{
	m_activeScene->render();
}