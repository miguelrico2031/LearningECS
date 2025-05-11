#pragma once
#include "BaseScene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Physics.h"

namespace ObjectComponent
{
	class Scene : public BaseScene
	{
	public:
		Scene() = default;

		inline Camera* getActiveCamera() const { return m_activeCamera; }

		virtual void load() override;
		virtual void unload() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;
		virtual void onWindowResize(int width, int height) override;

		GameObject* createGameObject();
		GameObject* createGameObject(const std::string& tag);

		void destroyGameObject(GameObject* gameObject);

		void onComponentAdded(GameObject* gameObject, Component* component);
		void onComponentRemoved(GameObject* gameObject, Component* component);

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
		std::vector<Camera*> m_cameras;
		Camera* m_activeCamera = nullptr;
		Physics m_physics;

	private:
		void findMainCamera();
	};
}