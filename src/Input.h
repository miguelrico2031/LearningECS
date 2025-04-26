#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <glm/vec2.hpp>

class Input
{
public:
    static void initialize(GLFWwindow* window);

    static bool isKeyPressed(int key);
    static bool isKeyReleased(int key);
    static bool isKeyHeld(int key);

    static bool isMouseButtonPressed(int button);
    static bool isMouseButtonReleased(int button);
    static bool isMouseButtonHeld(int button);

    static glm::vec2 getMousePosition();
    static glm::vec2 getMouseDelta();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void update();
private:
    static std::unordered_map<int, bool> currentKeys;
    static std::unordered_map<int, bool> previousKeys;

    static std::unordered_map<int, bool> currentMouseButtons;
    static std::unordered_map<int, bool> previousMouseButtons;

    static glm::vec2 mousePosition;
    static glm::vec2 mouseDelta;
    static glm::vec2 lastMousePosition;
};
