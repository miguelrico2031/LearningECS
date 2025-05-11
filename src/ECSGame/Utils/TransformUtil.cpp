#include "TransformUtil.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ECSGame
{
	glm::vec3 TransformUtil::getForward(const Transform& transform)
	{
		return glm::normalize(glm::rotate(glm::quat(glm::radians(transform.m_rotation)), { 0.0f, 0.0f, -1.0f }));
	}

	glm::vec3 TransformUtil::getRight(const Transform& transform)
	{
		return glm::normalize(glm::rotate(glm::quat(glm::radians(transform.m_rotation)), { 1.0f, 0.0f, 0.0f }));
	}

	glm::vec3 TransformUtil::getUp(const Transform& transform)
	{
		return glm::normalize(glm::rotate(glm::quat(glm::radians(transform.m_rotation)), { 0.0f, 1.0f, 0.0f }));
	}

	glm::mat4 TransformUtil::calculateModelMatrix(const Transform& transform)
	{

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.m_position);
		glm::quat rotationQuat = glm::quat(glm::radians(transform.m_rotation));
		glm::mat4 rotation = glm::toMat4(rotationQuat);
		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), transform.m_scale);

		return translation * rotation * scaling;
	}
}
