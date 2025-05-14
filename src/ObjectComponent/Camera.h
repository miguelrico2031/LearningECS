#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "const.h"

namespace ObjectComponent
{
	class Camera : public Component
	{
	public:
		enum class Projection : uint8_t { Orthographic, Perspective };
	public:
		Camera(GameObject* gameObject)
			: Component(gameObject),
			m_aspectRatio((float)WINDOW::WIDTH / (float)WINDOW::HEIGHT),
			m_projectionType(Projection::Perspective),
			m_verticalFOV(glm::radians(CAMERA::V_FOV_DEGREES)),
			m_near(CAMERA::NEAR), m_far(CAMERA::FAR),
			m_orthographicSize(CAMERA::ORTHO_SIZE),
			m_clearColor(CAMERA::CLEAR_COLOR_1),
			m_projDirty(true)
		{
		}

		inline Projection getProjectionType() const { return m_projectionType; }
		inline float getAspectRatio() const { return m_aspectRatio; }
		inline float getVerticalFOV() const { return m_verticalFOV; }
		inline float getNearPlane() const { return m_near; }
		inline float getFarPlane() const { return m_far; }
		inline float getOrthographicSize() const { return m_orthographicSize; }
		inline glm::vec4 getClearColor() const { return m_clearColor; }

		inline void setAspectRatio(float ar) { m_aspectRatio = ar; m_projDirty = true; }
		inline void setProjectionType(Projection projType) { m_projectionType = projType; m_projDirty = true; }
		inline void setVerticalFOV(float vFOV) { m_verticalFOV = vFOV; m_projDirty = true; }
		inline void setNearPlane(float near) { m_near = near; m_projDirty = true; }
		inline void setFarPlane(float far) { m_far = far; m_projDirty = true; }
		inline void setOrthographicSize(float size) { m_orthographicSize = size; m_projDirty = true; }
		inline void setClearColor(glm::vec4 clearColor) { m_clearColor = clearColor; }

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;
	private:
		mutable glm::mat4 m_projMatrix{};
		glm::vec4 m_clearColor;
		float m_aspectRatio, m_verticalFOV, m_near, m_far, m_orthographicSize;
		Projection m_projectionType;
		mutable bool m_projDirty;
	private:
		void updateProjectionMatrix() const;
	};
}