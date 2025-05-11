#include "PhysicsSystem.h"
#include "ECSGame/Components/Transform.h"
#include "ECSGame/Components/Collider.h"
#include "ECSGame/Components/Rigidbody.h"
#include "GameTime.h"
#include "ECSGame/Utils/PhysicsUtil.h"

namespace ECSGame
{
	PhysicsSystem::PhysicsSystem() : ECS::System(ECS::getMask<Transform, Collider, Rigidbody>())
	{

	}

	void PhysicsSystem::initialize()
	{
		m_onColliderAddedEvent = m_registry->getComponentAddedEvent<Collider>().subscribe([this](ECS::Entity entity) { onColliderAdded(entity); });
		m_onRigidbodyAddedEvent = m_registry->getComponentAddedEvent<Rigidbody>().subscribe([this](ECS::Entity entity) { onRigidbodyAdded(entity); });
		m_onColliderRemovedEvent = m_registry->getComponentRemovedEvent<Collider>().subscribe([this](ECS::Entity entity) { onColliderRemoved(entity); });
		m_onRigidbodyRemovedEvent = m_registry->getComponentRemovedEvent<Rigidbody>().subscribe([this](ECS::Entity entity) { onRigidbodyRemoved(entity); });

		m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
		m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
		m_broadphase = std::make_unique<btDbvtBroadphase>();
		m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
			m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());
		m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	}

	PhysicsSystem::~PhysicsSystem()
	{
		m_registry->getComponentAddedEvent<Collider>().unsubscribe(m_onColliderAddedEvent);
		m_registry->getComponentAddedEvent<Rigidbody>().unsubscribe(m_onRigidbodyAddedEvent);
		m_registry->getComponentRemovedEvent<Collider>().unsubscribe(m_onColliderRemovedEvent);
		m_registry->getComponentRemovedEvent<Rigidbody>().unsubscribe(m_onRigidbodyRemovedEvent);
	}



	void PhysicsSystem::fixedUpdate()
	{
		for (auto entity : getEntities())
		{
			Transform& transform = m_registry->getComponent<Transform>(entity);
			Rigidbody& rb = m_registry->getComponent<Rigidbody>(entity);
			PhysicsUtil::updateBulletRigidbodyfromTransform(rb, transform);
		}
		//physics simulation step
		float timeStep = GameTime::getFixedDeltaTime();
		m_dynamicsWorld->stepSimulation(timeStep, 10);


		for (auto entity : getEntities())
		{
			Transform& transform = m_registry->getComponent<Transform>(entity);
			Rigidbody& rb = m_registry->getComponent<Rigidbody>(entity);
			PhysicsUtil::updateTransformFromBulletRigidbody(rb, transform);
		}
	}




	void PhysicsSystem::onColliderAdded(ECS::Entity entity)
	{
		Collider& collider = m_registry->getComponent<Collider>(entity);
		if (collider.m_type == Collider::Type::Box)
		{
			collider.m_btCollisionShape = new btBoxShape(btVector3(1, 1, 1)); //TODO: change to physicsutil
		}
		else if (collider.m_type == Collider::Type::Sphere)
		{
			collider.m_btCollisionShape = new btSphereShape(1.0f);
		}
	}

	void PhysicsSystem::onRigidbodyAdded(ECS::Entity entity)
	{
		Rigidbody& rb = m_registry->getComponent<Rigidbody>(entity);

		assert(m_registry->hasComponent<Collider>(entity) && "Rigidbody needs a Collider component.");
		assert(m_registry->hasComponent<Transform>(entity) && "Rigidbody needs a Transform component.");


		Collider& collider = m_registry->getComponent<Collider>(entity);

		assert(collider.m_btCollisionShape != nullptr && "Rigidbody needs a Collider component with a valid btCollisionShape.");


		btVector3 localInertia(0, 0, 0);
		collider.m_btCollisionShape->calculateLocalInertia(1.0f, localInertia);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

		btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, motionState, collider.m_btCollisionShape, localInertia);
		rb.m_btRigidBody = new btRigidBody(rbInfo);

		Transform& transform = m_registry->getComponent<Transform>(entity);
		PhysicsUtil::updateBulletRigidbodyfromTransform(rb, transform);

		m_dynamicsWorld->addRigidBody(rb.m_btRigidBody);
	}

	void PhysicsSystem::onColliderRemoved(ECS::Entity entity)
	{
		Collider& collider = m_registry->getComponent<Collider>(entity);
		if (collider.m_btCollisionShape != nullptr)
		{
			delete collider.m_btCollisionShape;
			collider.m_btCollisionShape = nullptr;
		}
	}

	void PhysicsSystem::onRigidbodyRemoved(ECS::Entity entity)
	{
		Rigidbody& rb = m_registry->getComponent<Rigidbody>(entity);

		
		if (rb.m_btRigidBody != nullptr)
		{
			if (m_dynamicsWorld != nullptr)
			{
				m_dynamicsWorld->removeRigidBody(rb.m_btRigidBody);
			}

			if (rb.m_btRigidBody->getMotionState() != nullptr)
			{
				delete rb.m_btRigidBody->getMotionState();
			}
			delete rb.m_btRigidBody;
			rb.m_btRigidBody = nullptr;
		}
	}

	
}
