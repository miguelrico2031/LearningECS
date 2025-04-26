#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace ObjectComponent
{
	class Scene
	{
	public:
		Scene() = default;

		inline Camera* getActiveCamera() const { return m_activeCamera; }

		void load();
		void unload();
		void update(float deltaTime);
		void fixedUpdate();
		void render();

		GameObject* createGameObject();
		GameObject* createGameObject(const std::string& tag);

		void destroyGameObject(GameObject* gameObject);

		void onComponentAdded(GameObject* gameObject, Component* component);
		void onComponentRemoved(GameObject* gameObject, Component* component);

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
		std::vector<Camera*> m_cameras;
		Camera* m_activeCamera = nullptr;
	private:
		void findMainCamera();
	};
}