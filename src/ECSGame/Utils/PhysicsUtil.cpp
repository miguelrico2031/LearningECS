#include "PhysicsUtil.h"
#include "ECSGame/Components/Transform.h"
#include "ECSGame/Components/Collider.h"
#include "ECSGame/Components/Rigidbody.h"
#include <btBulletDynamicsCommon.h>
#include <cassert>

namespace ECSGame
{

	void PhysicsUtil::updateBulletRigidbodyfromTransform(Rigidbody& rb, Transform& transform)
	{
		btTransform bulletTransform;
		bulletTransform.setIdentity();
		bulletTransform.setOrigin(btVector3(transform.m_position.x, transform.m_position.y, transform.m_position.z));
		glm::vec3 rotRadians = glm::radians(transform.m_rotation);
		btQuaternion quat;
		quat.setEulerZYX(rotRadians.z, rotRadians.y, rotRadians.x);
		bulletTransform.setRotation(quat);
		rb.m_btRigidBody->setWorldTransform(bulletTransform);
	}

	void PhysicsUtil::updateTransformFromBulletRigidbody(Rigidbody& rb, Transform& transform)
	{
		btTransform bulletTransform = rb.m_btRigidBody->getWorldTransform();
		btVector3 position = bulletTransform.getOrigin();
		transform.m_position = { position.x(), position.y(), position.z() };
		btScalar yaw, pitch, roll;
		bulletTransform.getRotation().getEulerZYX(yaw, pitch, roll);
		glm::vec3 rotEulersRadians = { roll, pitch, yaw };
		transform.m_rotation = glm::degrees(rotEulersRadians);
	}

	void PhysicsUtil::setBoxColliderHalfExtents(Collider& collider, glm::vec3 halfExtents, Rigidbody* rb)
	{
		assert(collider.m_type == Collider::Type::Box && "Collider's type must be Box.");

		if (collider.m_btCollisionShape != nullptr)
		{
			delete collider.m_btCollisionShape;
		}
		collider.m_btCollisionShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
		if (rb != nullptr)
		{
			btVector3 localInertia(0, 0, 0);
			collider.m_btCollisionShape->calculateLocalInertia(rb->m_btRigidBody->getMass(), localInertia);
			rb->m_btRigidBody->setCollisionShape(collider.m_btCollisionShape);
			rb->m_btRigidBody->setMassProps(rb->m_btRigidBody->getMass(), localInertia);
		}
		
	}

	void PhysicsUtil::setSphereColliderRadius(Collider& collider, float radius, Rigidbody* rb)
	{
		assert(collider.m_type == Collider::Type::Sphere && "Collider's type must be Sphere.");

		if (collider.m_btCollisionShape != nullptr)
		{
			delete collider.m_btCollisionShape;
		}
		collider.m_btCollisionShape = new btSphereShape(radius);
		if (rb != nullptr)
		{
			btVector3 localInertia(0, 0, 0);
			collider.m_btCollisionShape->calculateLocalInertia(rb->m_btRigidBody->getMass(), localInertia);
			rb->m_btRigidBody->setCollisionShape(collider.m_btCollisionShape);
			rb->m_btRigidBody->setMassProps(rb->m_btRigidBody->getMass(), localInertia);
		}
	}

	void PhysicsUtil::setRigidbodyMass(Rigidbody& rb, float mass)
	{
		btVector3 localInertia(0, 0, 0);
		if (rb.m_btRigidBody->getCollisionShape() != nullptr)
		{
			rb.m_btRigidBody->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		}
		rb.m_btRigidBody->setMassProps(mass, localInertia);

	}
	void PhysicsUtil::setRigidbodyRestitution(Rigidbody& rb, float restitution)
	{
		rb.m_btRigidBody->setRestitution(restitution);
	}
}
