#pragma once
#include "ECS/ECS.h"
#include <btBulletDynamicsCommon.h>
#include <memory>

namespace ECSGame
{
	class Transform;
	class Rigidbody;
	class PhysicsSystem : public ECS::System
	{
	public:
		PhysicsSystem();
		void initialize();
		virtual ~PhysicsSystem() override;
		void fixedUpdate();
	private:
		void onColliderAdded(ECS::Entity entity);
		void onRigidbodyAdded(ECS::Entity entity);
		void onColliderRemoved(ECS::Entity entity);
		void onRigidbodyRemoved(ECS::Entity entity);
	private:
		EventHandlerID m_onColliderAddedEvent;
		EventHandlerID m_onRigidbodyAddedEvent;
		EventHandlerID m_onColliderRemovedEvent;
		EventHandlerID m_onRigidbodyRemovedEvent;

		std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> m_dispatcher;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	};
}