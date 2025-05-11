#include "FreeMovementSystem.h"
#include "Input.h"
#include "GameTime.h"
#include "ECSGame/Utils/TransformUtil.h"

namespace ECSGame
{
	void FreeMovementSystem::update()
	{
		glm::vec3 direction;
		bool sprint;
		bool newMovementInput = queryMovementKeysInput(direction, sprint);
		glm::vec2 mouseDelta = Input::getMouseDelta();
		float dt = GameTime::getDeltaTime();

		for (ECS::Entity e : getEntities())
		{
			const FreeMovement& freeMovement = m_registry->getComponent<FreeMovement>(e);
			Transform& transform = m_registry->getComponent<Transform>(e);

			//movement
			if (newMovementInput)
			{

				glm::vec3 movement = TransformUtil::getForward(transform) * direction.z +
					TransformUtil::getRight(transform) * direction.x +
					glm::vec3(0, 1, 0) * direction.y;
				float speed = sprint ? freeMovement.m_sprintSpeed : freeMovement.m_moveSpeed;
				transform.m_position += glm::normalize(movement) * speed * dt;
			}

			//rotation
			glm::vec2 rotationOffset = mouseDelta * freeMovement.m_rotationSensitivity * dt;
			transform.m_rotation.y-= rotationOffset.x;
			transform.m_rotation.x = glm::clamp(transform.m_rotation.x + rotationOffset.y, -89.0f, 89.0f);

		}
	}
	bool FreeMovementSystem::queryMovementKeysInput(glm::vec3& direction, bool& sprint)
	{
		bool newInput = false;
		direction = {};
		if (Input::isKeyHeld(GLFW_KEY_W))
		{
			direction.z += 1; newInput = true;
		}
		if (Input::isKeyHeld(GLFW_KEY_S))
		{
			direction.z -= 1; newInput = true;
		}
		if (Input::isKeyHeld(GLFW_KEY_A))
		{
			direction.x -= 1; newInput = true;
		}
		if (Input::isKeyHeld(GLFW_KEY_D))
		{
			direction.x += 1; newInput = true;
		}
		if (Input::isKeyHeld(GLFW_KEY_SPACE))
		{
			direction.y += 1; newInput = true;
		}
		if (Input::isKeyHeld(GLFW_KEY_LEFT_CONTROL))
		{
			direction.y -= 1; newInput = true;
		}

		if (!newInput)
			return false;

		sprint = Input::isKeyHeld(GLFW_KEY_LEFT_SHIFT);
		return true;
	}
}
