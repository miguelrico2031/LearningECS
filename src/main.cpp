#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "const.h"
#include "Log.h"
#include "Shader.h"
#include "Texture.h"
#include <cmath>


void onWindowResize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void createTriangle();


GLuint vbo;
GLuint ebo;
GLuint vao;


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


	// create a window object
	GLFWwindow* window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "LearningECS", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window\n";
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

	Shader shader("../assets/shaders/basic.vert", "../assets/shaders/basic.frag");
	Texture texture("../assets/textures/laura.png");
	GLint blueUniformLoc = shader.getLocation("blue");

	createTriangle();

	//loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//render calls
		glClearColor(0.1f, 0.05f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		shader.activate();
		float value = (sin(glfwGetTime() * 5.0f) + 1.0f) * 0.5f;
		//shader.setFloat(blueUniformLoc, value);
		texture.activate();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static bool wf = false;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		wf = !wf;
		glPolygonMode(GL_FRONT_AND_BACK, wf ? GL_LINE : GL_FILL);
	}

}

void createTriangle()
{
	float vertices[] =
	{	 // positions			 // colors				// uvs
		 0.5f,  0.5f,  0.0f,	 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.0f,	 0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.0f,	 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,	 1.0f, 1.0f, 0.0f,		0.0f, 1.0f 
	};


	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	//create vao to store how the attributes of the vbo will be read by openGL
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); //set vao as the active one


	glGenBuffers(1, &vbo); //create vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //set vbo as active one
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertices to active vbo in gpu

	//create ebo to store the indices
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//configure the active vao
	//position attribute:
	GLuint positionAttributeLocation = 0u;  //the location specificated in the vertex shader
	glVertexAttribPointer(positionAttributeLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(positionAttributeLocation); //enable the attribute

	//color attribute:
	GLuint colorAttributeLocation = 1u;  //the location specificated in the vertex shader
	glVertexAttribPointer(colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colorAttributeLocation); //enable the attribute


	//uv attribute:
	GLuint uvAttributeLocation = 2u;  //the location specificated in the vertex shader
	glVertexAttribPointer(uvAttributeLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(uvAttributeLocation); //enable the attribute
}


