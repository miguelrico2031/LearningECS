#include "Camera.h"
#include "Transform.h"
namespace ObjectComponent
{
    glm::mat4 Camera::getViewMatrix() const
    {
        Transform* transform = m_gameObject->getTransform();
        return glm::lookAt(transform->getPosition(), 
                           transform->getPosition() + transform->getForward(), 
                           transform->getUp());
    }


    glm::mat4 Camera::getProjectionMatrix() const
    {
        updateProjectionMatrix();
        return m_projMatrix;
    }


    void Camera::updateProjectionMatrix() const
    {
        if (!m_projDirty) return;

        if (m_projectionType == Projection::Perspective)
        {
            m_projMatrix = glm::perspective(m_verticalFOV, m_aspectRatio, m_near, m_far);
        }
        else // orthographic projection
        {
            float left = -m_orthographicSize * m_aspectRatio * 0.5f;
            float right = m_orthographicSize * m_aspectRatio * 0.5f;
            float bottom = -m_orthographicSize * 0.5f;
            float top = m_orthographicSize * 0.5f;

            m_projMatrix = glm::ortho(left, right, bottom, top, m_near, m_far);
        }

        m_projDirty = false;
    }
}