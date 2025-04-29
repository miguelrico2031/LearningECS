#include "Rigidbody.h"
#include "Collider.h"
#include "Log.h"
#include "Transform.h"
#include <glm/gtc/type_ptr.hpp>

namespace ObjectComponent
{
	Rigidbody::Rigidbody(GameObject* gameObject)
		: Component(gameObject)
	{
		Collider* collider = gameObject->getComponent<Collider>();
		if (collider == nullptr)
		{
			LOG_ERROR("Tried to create rigidbody in gameObject with no Collider component!");
			return;
		}

		btVector3 localInertia(0, 0, 0);
		collider->getCollisionShape()->calculateLocalInertia(1.0f, localInertia);
		m_motionState = std::make_unique<btDefaultMotionState>(
			btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

		btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_motionState.get(),
			collider->getCollisionShape(), localInertia);
		m_rigidbody = std::make_unique<btRigidBody>(rbInfo);
		updateRigidbodyBeforeStep();
	}

	void Rigidbody::updateRigidbodyBeforeStep()
	{
		btTransform bulletTransform;
		bulletTransform.setIdentity();
		Transform* transform = m_gameObject->getTransform();
		//glm::mat4 modelMatrix = transform->getModelMatrix();
		//bulletTransform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

		bulletTransform.setOrigin(btVector3(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z));
		auto og = bulletTransform.getOrigin();
		glm::vec3 rotRadians = glm::radians(transform->getRotation());
		btQuaternion quat;
		quat.setEulerZYX(rotRadians.z, rotRadians.y, rotRadians.x),
		bulletTransform.setRotation(quat);

		m_rigidbody->setWorldTransform(bulletTransform);
	}

	void Rigidbody::updateTransformAfterStep()
	{
		btTransform bulletTransform = m_rigidbody->getWorldTransform();
		//m_motionState->getWorldTransform(bulletTransform);
		Transform* transform = m_gameObject->getTransform();
		btVector3 position = bulletTransform.getOrigin();
		transform->setPosition({ position.x(), position.y(), position.z() });
		//glm::vec3 rotEulers;
		//bulletTransform.getRotation().getEulerZYX(rotEulers.y, rotEulers.x, rotEulers.);
		//glm::vec3 rotEulersDegres = glm::degrees(rotEulers);
		//transform->setRotation(rotEulersDegres);

		btScalar yaw, pitch, roll;
		bulletTransform.getRotation().getEulerZYX(yaw, pitch, roll);
		glm::vec3 rotEulersDegrees = glm::degrees(glm::vec3(roll, pitch, yaw));
		transform->setRotation(rotEulersDegrees);
	}
}
