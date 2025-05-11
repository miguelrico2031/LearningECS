#pragma once
#include "const.h"
#include <glm/glm.hpp>

namespace ECSGame
{
	struct Camera
	{
		mutable glm::mat4 m_projMatrix = {};
		glm::vec4 m_clearColor = CAMERA::CLEAR_COLOR;
		float m_aspectRatio = (float)WINDOW::WIDTH / (float)WINDOW::HEIGHT;
		float m_verticalFOV = glm::radians(CAMERA::V_FOV_DEGREES);
		float m_near = CAMERA::NEAR;
		float m_far = CAMERA::FAR;
		float m_orthographicSize = CAMERA::ORTHO_SIZE;
		enum class Projection : uint8_t { Orthographic, Perspective };
		Projection m_projectionType = Projection::Perspective;
		mutable bool m_projDirty = true;
	};
}