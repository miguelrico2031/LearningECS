#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "const.h"
#include "Log.h"
#include "ObjectComponent/Shader.h"
#include "ObjectComponent/Texture.h"
#include "ObjectComponent/GameObject.h"
#include "ObjectComponent/Transform.h"
#include "ObjectComponent/Camera.h"
#include "ObjectComponent/MeshRenderer.h"
#include "ObjectComponent/Primitives.h"

using namespace ObjectComponent;

float deltaTime = 0.0f, lastFrameTime = 0.0f;
constexpr size_t N_CUBES = 4;
GameObject cameraGO("Main Camera");
struct
{
	float speed = 5.0f;
	float sensitivity = 5.0f;
} cameraMoveParams;


void onWindowResize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processCursorPosition(GLFWwindow* window, double xpos, double ypos);



int main()
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
	GLFWwindow* window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "LearningECS", NULL, NULL);
	if (window == NULL)
	{
		LOG_ERROR("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init GLAD (loads all OpenGL function pointers)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to init GLAD");
		return -1;
	}

	//configure rendering window size and resize callback
	glViewport(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, processCursorPosition);

	//config OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);       // Enable face culling
	glCullFace(GL_BACK);          // Discards back faces
	glFrontFace(GL_CCW);          // counter clock wise is front


	Camera* camera = cameraGO.addComponent<Camera>();
	cameraGO.getTransform()->setPosition({ 0, 0, 3 });
	std::vector<GameObject> cubes(N_CUBES);
	std::shared_ptr<Material> material = std::make_shared<Material>("../assets/shaders/basic.vert", 
																	"../assets/shaders/basic.frag",
																	"../assets/textures/laura.png");

	std::shared_ptr<Mesh> mesh = Primitives::createSphereMesh(16, 4);

	std::vector<glm::vec3> positions =
	{
		{ 5, 0,  0},
		{-5, 0,  0},
		{ 0, 0,  5},
		{ 0, 0, -5}
	};

	for (size_t i = 0; i < cubes.size(); i++)
	{
		GameObject& cube = cubes[i];
		cube.setTag("Cube " + i);
		Transform* transform = cube.getTransform();
		transform->setPosition(positions[i]);
		transform->setRotation({ 0, 90, -90 });
		MeshRenderer* meshRenderer = cube.addComponent<MeshRenderer>();
		meshRenderer->setMesh(mesh);
		meshRenderer->setMaterial(material);
	}

	//loop
	while (!glfwWindowShouldClose(window))
	{
		float frameTime = (float)glfwGetTime();
		deltaTime = frameTime - lastFrameTime;
		lastFrameTime = frameTime;

		processInput(window);

		//render calls
		glClearColor(0.1f, 0.05f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 viewProjection = camera->getProjectionMatrix() * camera->getViewMatrix();
		for (auto& cube : cubes)
		{
			MeshRenderer* meshRenderer = cube.getComponent<MeshRenderer>();
			if(meshRenderer != nullptr)
				meshRenderer->render(viewProjection);
		}

		glfwPollEvents(); // check and call events
		glfwSwapBuffers(window); // swap buffers
	}



	//clean GLFW resources
	glfwTerminate();
	return 0;
}

void onWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Camera* camera = cameraGO.getComponent<Camera>();
	camera->setAspectRatio((float)width / (float)height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 moveInput = { 0, 0, 0 };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveInput.y += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveInput.y -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveInput.x -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveInput.x += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		moveInput.z += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		moveInput.z -= 1;
	}

	if (glm::length(moveInput) > 0.0f)
	{
		Transform* ct = cameraGO.getTransform();
		glm::vec3 movement = ct->getForward() * moveInput.y +
							 ct->getRight()   * moveInput.x +
							 glm::vec3(0, 1, 0)	  * moveInput.z;
		//movement.y = 0.0f; //para no moverse del suelo
		
		ct->setPosition( ct->getPosition() + glm::normalize(movement) * cameraMoveParams.speed * deltaTime);

		LOG("Cam pos: " << ct->getPosition().x << ", " << ct->getPosition().y << ", " << ct->getPosition().z);
	}


}

void processCursorPosition(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = WINDOW::WIDTH / 2.0;
	static float lastY = WINDOW::HEIGHT / 2.0;
	static bool firstMouse = true;
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = lastX - xpos;
	float yOffset = lastY - ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;
	xOffset *= cameraMoveParams.sensitivity * deltaTime;
	yOffset *= cameraMoveParams.sensitivity * deltaTime;

	glm::vec3 rot = cameraGO.getTransform()->getRotation();
	rot.y += xOffset;
	rot.x = glm::clamp(rot.x + yOffset, -89.0f, 89.0f);
	cameraGO.getTransform()->setRotation(rot);
}


