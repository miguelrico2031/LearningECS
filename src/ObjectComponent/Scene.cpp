#include "Scene.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
namespace ObjectComponent
{
	void Scene::load()
	{
		m_physics.initialize();
	}

	void Scene::unload()
	{
	}

	void Scene::update(float deltaTime)
	{
	}

	void Scene::fixedUpdate()
	{
		m_physics.fixedUpdate();
	}

	void Scene::render()
	{
		if (m_activeCamera == nullptr || !m_activeCamera->getEnabled())
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			return;
		}

		glm::vec4 clearColor = m_activeCamera->getClearColor();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 viewProjection = m_activeCamera->getProjectionMatrix() * m_activeCamera->getViewMatrix();
		for (auto& gameObject : m_gameObjects)
		{
			MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
			if (meshRenderer != nullptr && meshRenderer->getEnabled())
				meshRenderer->render(viewProjection);
		}
	}

	GameObject* Scene::createGameObject()
	{
		m_gameObjects.push_back(std::make_unique<GameObject>(this));
		return m_gameObjects.back().get();
	}

	GameObject* Scene::createGameObject(const std::string& tag)
	{
		m_gameObjects.push_back(std::make_unique<GameObject>(this, tag));
		return m_gameObjects.back().get();
	}

	void Scene::destroyGameObject(GameObject* gameObject)
	{
		if (Camera* cam = gameObject->getComponent<Camera>())
		{
			m_cameras.erase(std::remove(m_cameras.begin(), m_cameras.end(), cam), m_cameras.end());
			if (m_activeCamera == cam)
			{
				findMainCamera();
			}
		}

		auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
			[gameObject](const std::unique_ptr<GameObject>& obj) { return obj.get() == gameObject; });

		if (it != m_gameObjects.end())
		{
			m_gameObjects.erase(it);
		}
	}

	void Scene::onComponentAdded(GameObject* gameObject, Component* component)
	{
		if (Camera* cam = dynamic_cast<Camera*>(component))
		{
			m_cameras.push_back(cam);
			m_activeCamera = cam;
		}
		else if (Rigidbody* rb = dynamic_cast<Rigidbody*>(component))
		{
			m_physics.addRigidbody(rb);
		}
	}

	void Scene::onComponentRemoved(GameObject* gameObject, Component* component)
	{
		if (Camera* cam = dynamic_cast<Camera*>(component))
		{
			m_cameras.erase(std::remove(m_cameras.begin(), m_cameras.end(), cam), m_cameras.end());
			if (m_activeCamera == cam)
			{
				findMainCamera();
			}
		}
		else if (Rigidbody* rb = dynamic_cast<Rigidbody*>(component))
		{
			m_physics.removeRigidbody(rb);
		}
	}
	void Scene::findMainCamera()
	{
		if (m_cameras.empty())
		{
			m_activeCamera = nullptr;
		}
		else
		{
			m_activeCamera = m_cameras.back();
		}
	}
}
