#include "Input.h"
#include "const.h"
#include "GameTime.h"

std::unordered_map<int, bool> Input::currentKeys;
std::unordered_map<int, bool> Input::previousKeys;

std::unordered_map<int, bool> Input::currentMouseButtons;
std::unordered_map<int, bool> Input::previousMouseButtons;

glm::vec2 Input::mousePosition = { 0.0f, 0.0f };
glm::vec2 Input::mouseDelta = { 0.0f, 0.0f };
glm::vec2 Input::lastMousePosition = { 0.0f, 0.0f };

void Input::initialize(GLFWwindow* window)
{
	lastMousePosition = { WINDOW::WIDTH / 2.0f, WINDOW::HEIGHT / 2.0f };
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
}

bool Input::isKeyPressed(int key)
{
	return currentKeys[key] && !previousKeys[key];
}

bool Input::isKeyReleased(int key)
{
	return !currentKeys[key] && previousKeys[key];
}

bool Input::isKeyHeld(int key)
{
	return currentKeys[key];
}

bool Input::isMouseButtonPressed(int button)
{
	return currentMouseButtons[button] && !previousMouseButtons[button];
}

bool Input::isMouseButtonReleased(int button)
{
	return !currentMouseButtons[button] && previousMouseButtons[button];
}

bool Input::isMouseButtonHeld(int button)
{
	return currentMouseButtons[button];
}

glm::vec2 Input::getMousePosition()
{
	return mousePosition;
}

glm::vec2 Input::getMouseDelta()
{
	return mouseDelta;
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Update the current key state based on the action
	if (action == GLFW_PRESS) {
		currentKeys[key] = true;
	} else if (action == GLFW_RELEASE) {
		currentKeys[key] = false;
	}
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	previousMouseButtons[button] = currentMouseButtons[button];
	currentMouseButtons[button] = (action != GLFW_RELEASE);
}

void Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool first = true;
	if (first)
	{
		lastMousePosition = { (float)xpos, (float)ypos };
		first = false;
	}
	mousePosition = { (float)xpos, (float)ypos };
	mouseDelta =  mousePosition - lastMousePosition;
	mouseDelta.y *= -1;
	float deltaTime = GameTime::getDeltaTime(true);
	if (deltaTime > 0.0f)
	{
		mouseDelta /= deltaTime;
	}
	lastMousePosition = mousePosition;
}

void Input::update()
{
	// Synchronize previousKeys with currentKeys for the next frame
	previousKeys = currentKeys;

	// Update mouse button states similarly if needed
	previousMouseButtons = currentMouseButtons;

	mouseDelta = { 0, 0 };
}
