#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "const.h"
#include "Log.h"
#include "Shader.h"
#include "Texture.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 view;
glm::mat4 proj;

float deltaTime = 0.0f, lastFrameTime = 0.0f;

struct Camera
{
	glm::vec3 position = { 0.0f, 0.0f, -3.0f };
	glm::vec3 forward = { 0.0f, 0.0f, -1.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 eulerAngles = { 0.0f, -90.0f, 0.0f };
	float speed = 5.0f;
	float sensitivity = 0.1f;
};

Camera camera;

struct Shape
{
	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	GLsizei verticesCount;
	GLsizei indicesCount;
	glm::mat4 transform = glm::mat4(1.0f);
};
void onWindowResize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processCursorPosition(GLFWwindow* window, double xpos, double ypos);

Shape createShape(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize);
Shape createQuad();
Shape createCube();


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

	glEnable(GL_CULL_FACE);       // Activa el face culling
	glCullFace(GL_BACK);          // Descarta las caras traseras (por defecto)
	glFrontFace(GL_CCW);          // Considera antihorario (CCW) como frente (por defecto)

	Shader shader("../assets/shaders/basic.vert", "../assets/shaders/basic.frag");
	Texture texture("../assets/textures/laura.png");
	GLint blueUniformLoc = shader.getLocation("blue");

	proj = glm::perspective(glm::radians(45.0f), (float)WINDOW::WIDTH / (float)WINDOW::HEIGHT, 0.1f, 100.0f);

	Shape shapes[] =
	{
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
		createCube(),
	};

	float xOffset = -10.0f;
	bool offsetInY = false;
	for (Shape& shape : shapes)
	{
		shape.transform = glm::translate(shape.transform, 
			glm::vec3(xOffset, (offsetInY ? 1.5f : 0.0f), -10.0f));
		xOffset += 2.0f;
		offsetInY = !offsetInY;
	}

	GLuint modelViewProjLocation = glGetUniformLocation(shader.getID(), "modelViewProj");




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

		view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
		float value = (sin(glfwGetTime() * 5.0f) + 1.0f) * 0.5f;


		shader.activate();
		texture.activate();
		shader.setFloat(blueUniformLoc, value);
		
		for (Shape& shape : shapes)
		{
			shape.transform = glm::rotate(shape.transform, .001f, glm::normalize(glm::vec3(1, 1, 0)));
			shader.setMat4(modelViewProjLocation, proj * view * shape.transform);
			glBindVertexArray(shape.vao);
			glDrawElements(GL_TRIANGLES, shape.indicesCount, GL_UNSIGNED_INT, 0);
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
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
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

	glm::vec3 movement = { 0, 0, 0 };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movement += camera.forward;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		movement -= camera.forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movement -= glm::normalize(glm::cross(camera.forward, camera.up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movement += glm::normalize(glm::cross(camera.forward, camera.up));
	}

	if (glm::length(movement) > 0.0f)
	{
		camera.position += glm::normalize(movement) * camera.speed * deltaTime;
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

	float xOffset = (float)xpos - lastX;
	float yOffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;
	xOffset *= camera.sensitivity;
	yOffset *= camera.sensitivity;

	camera.eulerAngles.y += xOffset;
	camera.eulerAngles.x = glm::clamp(camera.eulerAngles.x + yOffset, -89.0f, 89.0f);

	camera.forward.x = cos(glm::radians(camera.eulerAngles.y)) * cos(glm::radians(camera.eulerAngles.x));
	camera.forward.y = sin(glm::radians(camera.eulerAngles.x));
	camera.forward.z = sin(glm::radians(camera.eulerAngles.y)) * cos(glm::radians(camera.eulerAngles.x));
	camera.forward = glm::normalize(camera.forward);
}



//vertices in this format (for every vertex:) posx, posy, posz, colorR, colorG, colorB, uvX, uvY
Shape createShape(float* vertices, size_t verticesCount, unsigned int* indices, size_t indicesCount)
{
	Shape shape{};
	shape.verticesCount = verticesCount;
	shape.indicesCount = indicesCount;

	//create vao to store how the attributes of the vbo will be read by openGL
	glGenVertexArrays(1, &shape.vao);
	glBindVertexArray(shape.vao); //set vao as the active one


	glGenBuffers(1, &shape.vbo); //create vbo
	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo); //set vbo as active one
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float), vertices, GL_STATIC_DRAW); //copy vertices to active vbo in gpu

	//create ebo to store the indices
	glGenBuffers(1, &shape.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

	return shape;
}

Shape createQuad()
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

	return createShape(vertices, 32, indices, 6);
}

Shape createCube()
{
	float vertices[] = {
		// Front face
		-0.5f, -0.5f,  0.5f,  1, 1, 1,  0, 0,
		 0.5f, -0.5f,  0.5f,  1, 1, 1,  1, 0,
		 0.5f,  0.5f,  0.5f,  1, 1, 1,  1, 1,
		-0.5f,  0.5f,  0.5f,  1, 1, 1,  0, 1,

		// Back face
		 0.5f, -0.5f, -0.5f,  1, 1, 1,  0, 0,
		-0.5f, -0.5f, -0.5f,  1, 1, 1,  1, 0,
		-0.5f,  0.5f, -0.5f,  1, 1, 1,  1, 1,
		 0.5f,  0.5f, -0.5f,  1, 1, 1,  0, 1,

		 // Left face
		 -0.5f, -0.5f, -0.5f,  1, 1, 1,  0, 0,
		 -0.5f, -0.5f,  0.5f,  1, 1, 1,  1, 0,
		 -0.5f,  0.5f,  0.5f,  1, 1, 1,  1, 1,
		 -0.5f,  0.5f, -0.5f,  1, 1, 1,  0, 1,

		 // Right face
		  0.5f, -0.5f,  0.5f,  1, 1, 1,  0, 0,
		  0.5f, -0.5f, -0.5f,  1, 1, 1,  1, 0,
		  0.5f,  0.5f, -0.5f,  1, 1, 1,  1, 1,
		  0.5f,  0.5f,  0.5f,  1, 1, 1,  0, 1,

		  // Top face
		  -0.5f,  0.5f,  0.5f,  1, 1, 1,  0, 0,
		   0.5f,  0.5f,  0.5f,  1, 1, 1,  1, 0,
		   0.5f,  0.5f, -0.5f,  1, 1, 1,  1, 1,
		  -0.5f,  0.5f, -0.5f,  1, 1, 1,  0, 1,

		  // Bottom face
		  -0.5f, -0.5f, -0.5f,  1, 1, 1,  0, 0,
		   0.5f, -0.5f, -0.5f,  1, 1, 1,  1, 0,
		   0.5f, -0.5f,  0.5f,  1, 1, 1,  1, 1,
		  -0.5f, -0.5f,  0.5f,  1, 1, 1,  0, 1,
	};




	unsigned int indices[] = {
	0,  1,  2,  2,  3,  0,      // Front
	4,  5,  6,  6,  7,  4,      // Back
	8,  9, 10, 10, 11,  8,      // Left
   12, 13, 14, 14, 15, 12,      // Right
   16, 17, 18, 18, 19, 16,      // Top
   20, 21, 22, 22, 23, 20       // Bottom
	};





	return createShape(vertices, 192, indices, 36);
}


