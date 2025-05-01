#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace ObjectComponent
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		Transform(GameObject* gameObject, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
			: Component(gameObject), m_position(position), m_rotation(rotation), m_scale(scale)
		{
		}

		inline glm::vec3 getPosition() const { return m_position; }
		inline glm::vec3 getRotation() const { return m_rotation; }
		inline glm::vec3 getScale()    const { return m_scale; }
		inline glm::mat4 getModelMatrix()    const { updateModelMatrix(); return m_modelMatrix; }
		glm::vec3 getForward()  const;
		glm::vec3 getRight()  const;
		glm::vec3 getUp()  const;

		void setPosition(glm::vec3 position) { m_position = position; m_dirty = true; }
		void setRotation(glm::vec3 rotation) { m_rotation = rotation; m_dirty = true; }
		void setScale(glm::vec3 scale) { m_scale = scale; m_dirty = true; }

		void translate(glm::vec3 translation) { m_position += translation; m_dirty = true; }

	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
		mutable glm::mat4 m_modelMatrix = glm::mat4(1.0f);
		mutable bool m_dirty = false;
	private:
		void updateModelMatrix() const;
	};
}