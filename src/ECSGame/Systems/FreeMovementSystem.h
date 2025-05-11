#pragma once
#include "ECSGame/Components/FreeMovement.h"
#include "ECSGame/Components/Transform.h"
#include "ECS/ECS.h"

namespace ECSGame
{
	class FreeMovementSystem : public ECS::System
	{
	public:
		FreeMovementSystem() : ECS::System(ECS::getMask<Transform, FreeMovement>()) {}

		void update();
	private:
		bool queryMovementKeysInput(glm::vec3& direction, bool& sprint);

	};
}