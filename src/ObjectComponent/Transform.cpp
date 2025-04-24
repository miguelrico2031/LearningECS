#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ObjectComponent
{

	glm::vec3 Transform::getForward() const
	{
		return glm::normalize(glm::rotate(glm::quat(m_rotation), { 0.0f, 0.0f, -1.0f }));
	}

	glm::vec3 Transform::getRight() const
	{
		return glm::normalize(glm::rotate(glm::quat(m_rotation), { 1.0f, 0.0f, 0.0f }));
	}

	glm::vec3 Transform::getUp() const
	{
		return glm::normalize(glm::rotate(glm::quat(m_rotation), { 0.0f, 1.0f, 0.0f }));
	}

	void Transform::updateModelMatrix() const
	{
		if (!m_dirty) return;

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
		glm::quat rotationQuat = glm::quat(glm::radians(m_rotation));
		glm::mat4 rotation = glm::toMat4(rotationQuat);
		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), m_scale);

		m_modelMatrix = translation * rotation * scaling;

		m_dirty = false;
	}
}