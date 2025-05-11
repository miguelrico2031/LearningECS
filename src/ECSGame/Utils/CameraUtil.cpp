#include "CameraUtil.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ECSGame
{
	void CameraUtil::updateProjectionMatrix(const ECSGame::Camera& camera)
	{
		if (!camera.m_projDirty) return;

		if (camera.m_projectionType == Camera::Projection::Perspective)
		{
			camera.m_projMatrix = glm::perspective(camera.m_verticalFOV, camera.m_aspectRatio, camera.m_near, camera.m_far);
		}
		else // orthographic projection
		{
			float left = -camera.m_orthographicSize * camera.m_aspectRatio * 0.5f;
			float right = camera.m_orthographicSize * camera.m_aspectRatio * 0.5f;
			float bottom = -camera.m_orthographicSize * 0.5f;
			float top = camera.m_orthographicSize * 0.5f;

			camera.m_projMatrix = glm::ortho(left, right, bottom, top, camera.m_near, camera.m_far);
		}

		camera.m_projDirty = false;
	}

}