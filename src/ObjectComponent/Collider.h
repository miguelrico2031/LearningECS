#pragma once
#include "Component.h"
#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>

namespace ObjectComponent
{
    class Collider : public Component
    {
    public:
        Collider(GameObject* gameObject) : Component(gameObject) {}
        virtual ~Collider() = default;

        btCollisionShape* getCollisionShape() const { return m_collisionShape.get(); }

    protected:
        std::unique_ptr<btCollisionShape> m_collisionShape = nullptr;
    };

    class BoxCollider : public Collider
    {
    public:
        BoxCollider(GameObject* gameObject)
            : Collider(gameObject)
        {
        }

        void setHalfExtents(const glm::vec3& halfExtents)
        {
            m_collisionShape = std::make_unique<btBoxShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
        }
    };

    class SphereCollider : public Collider
    {
    public:
        SphereCollider(GameObject* gameObject)
            : Collider(gameObject)
        {
        }

        void setRadius(float radius)
        {
            m_collisionShape = std::make_unique<btSphereShape>(radius);
        }
    };
}
