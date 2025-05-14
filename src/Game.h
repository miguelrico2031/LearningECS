#pragma once
#include "BaseScene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>



class BaseEditor;
class Game
{
public:
	Game() {}

	inline BaseScene* getScene() const { return m_activeScene.get(); }
	inline GLFWwindow* getWindow() const { return m_window; }

	void attachEditor(BaseEditor* editor);

	template <class Scene_T>
	Scene_T* createScene()
	{
		static_assert(std::is_base_of<BaseScene, Scene_T>::value, "Scene_T must inherit from BaseScene");
		if (m_activeScene != nullptr)
		{
			m_activeScene->unload();
		}
		m_activeScene = std::make_unique<Scene_T>();
		m_activeScene->load();
		return static_cast<Scene_T*>(m_activeScene.get());
	}


	void setUp();
	void runLoop();
	void dispose();

	void quit();

private:
	Game(const Game& g) = delete;
	bool setUpRenderWindow();
	void onWindowResize(GLFWwindow* window, int width, int height);

	void update();
	void fixedUpdate();
	void render();

private:
	GLFWwindow* m_window = nullptr; //TODO: could be unique_ptr ??
	std::unique_ptr<BaseScene> m_activeScene = nullptr;
	BaseEditor* m_editor = nullptr;
	float m_deltaTime = 0.0f, m_lastFrameTime = 0.0f, m_accumulatedTime = 0.0f;


};