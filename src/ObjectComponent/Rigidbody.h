#pragma once
#include "Component.h"
#include <btBulletDynamicsCommon.h>

namespace ObjectComponent
{
    class Rigidbody : public Component
    {
    public:
        Rigidbody(GameObject* gameObject);

        btRigidBody* getBulletRigidbody() const { return m_rigidbody.get(); }
        void updateRigidbodyBeforeStep();
        void updateTransformAfterStep();


    private:
        std::unique_ptr<btRigidBody> m_rigidbody;
        std::unique_ptr<btMotionState> m_motionState;
    };
}
