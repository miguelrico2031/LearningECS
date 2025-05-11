#pragma once
#include <glm/glm.hpp>

namespace ECSGame
{
	class Rigidbody;
	class Collider;
	class Transform;
	class PhysicsUtil
	{
	public:
		static void updateBulletRigidbodyfromTransform(Rigidbody& rb, Transform& transform);
		static void updateTransformFromBulletRigidbody(Rigidbody& rb, Transform& transform);

		//if collider entity has rigidbody it must be provided in the rb pointer
		static void setBoxColliderHalfExtents(Collider& collider, glm::vec3 halfExtents, Rigidbody* rb = nullptr);
		static void setSphereColliderRadius(Collider& collider, float radius, Rigidbody* rb = nullptr);

		//zero mass means static
		static void setRigidbodyMass(Rigidbody& rb, float mass);
		static void setRigidbodyRestitution(Rigidbody& rb, float restitution);

	};
}