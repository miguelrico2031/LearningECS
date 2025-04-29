#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Editor.h"
#include "ObjectComponent/Scene.h"



class Game
{
public:

	inline static Game& get()
	{
		return s_singleton;
	}

	inline ObjectComponent::Scene* getScene() const { return m_activeScene.get(); }
	inline GLFWwindow* getWindow() const { return m_window; }

	inline void attachEditor(Editor* editor) { m_editor = editor; m_editor->onAttach(); }

	ObjectComponent::Scene* createScene();


	void setUp();
	void runLoop();
	void dispose();


private:
	static Game s_singleton;
	GLFWwindow* m_window = nullptr; //TODO: could be unique_ptr ??

	std::unique_ptr<ObjectComponent::Scene> m_activeScene = nullptr;

	Editor* m_editor = nullptr;


	float m_deltaTime = 0.0f, m_lastFrameTime = 0.0f, m_accumulatedTime = 0.0f;
private:
	Game() {}
	Game(const Game& g) = delete;
	bool setUpRenderWindow();
	void disposeRenderWindow();
	static void onWindowResize(GLFWwindow* window, int width, int height);

	void update();
	void fixedUpdate();
	void render();

};