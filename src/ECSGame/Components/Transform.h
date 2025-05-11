#pragma once
#include <glm/glm.hpp>

namespace ECSGame
{
	struct Transform
	{
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}