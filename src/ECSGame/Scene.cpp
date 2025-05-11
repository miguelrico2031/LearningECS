#include "Scene.h"

#include "ECSGame/Components/Tag.h"
#include "ECSGame/Components/Transform.h"
#include "ECSGame/Components/Camera.h"
#include "ECSGame/Components/MeshRenderer.h"
#include "ECSGame/Components/Collider.h"
#include "ECSGame/Components/Rigidbody.h"

#include "ECSGame/Systems/RenderSystem.h"
#include "ECSGame/Systems/PhysicsSystem.h"

namespace ECSGame
{
	Scene::~Scene()
	{

	}

	void Scene::load()
	{
		Scene::registerAllComponentTypes();
		m_registry.initialize();

		m_renderCameraParamsSystem = m_registry.addSystem<RenderCameraParamsSystem>();
		m_renderSystem = m_registry.addSystem<RenderSystem>();

		m_physicsSystem = m_registry.addSystem<PhysicsSystem>();
		m_physicsSystem->initialize();
	}

	void Scene::unload()
	{
	}

	void Scene::update()
	{
	}

	void Scene::fixedUpdate()
	{
		m_physicsSystem->fixedUpdate();
	}

	void Scene::render()
	{
		glm::mat4 viewProjectionMatrix;
		glm::vec4 clearColor;
		bool cameraExists = m_renderCameraParamsSystem->getRenderCameraParams(viewProjectionMatrix, clearColor);

		if (!cameraExists)
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			return;
		}

		m_renderSystem->render(viewProjectionMatrix, clearColor);
	}

	void Scene::onWindowResize(int width, int height)
	{
	}


	void Scene::registerAllComponentTypes()
	{
		static bool done = false;
		if (done) return;

		ECS::registerComponentType<Tag>();
		ECS::registerComponentType<Transform>();
		ECS::registerComponentType<Camera>();
		ECS::registerComponentType<MeshRenderer>();
		ECS::registerComponentType<Collider>();
		ECS::registerComponentType<Rigidbody>();


		done = true;
	}
}