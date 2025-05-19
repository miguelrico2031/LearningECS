#pragma once 
#include "BaseScene.h"
#include "ECS/ECS.h"

namespace ECSGame
{
	class RenderSystem;
	class CameraSystem;
	class PhysicsSystem;
	class Scene : public BaseScene
	{
	public:
		Scene() = default;
		virtual ~Scene();

		inline ECS::Registry& getRegistry() { return m_registry; }
		inline const ECS::Registry& getRegistry() const { return m_registry; }

		virtual void load() override;
		virtual void unload() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;
		virtual void onWindowResize(int width, int height) override;
	private:
		ECS::Registry m_registry;
		CameraSystem* m_cameraSystem = nullptr;
		RenderSystem* m_renderSystem = nullptr;
		PhysicsSystem* m_physicsSystem = nullptr;
	private:
		static void registerAllComponentTypes();
	};
};