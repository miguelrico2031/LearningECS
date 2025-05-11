#pragma once
#include "ECSGame/Components/Transform.h"

namespace ECSGame
{
	class TransformUtil
	{
	public:
		static glm::vec3 getForward(const Transform& transform);
		static glm::vec3 getRight(const Transform& transform);
		static glm::vec3 getUp(const Transform& transform);
		static glm::mat4 calculateModelMatrix(const Transform& transform);
	};
}