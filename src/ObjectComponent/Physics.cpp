#include "Physics.h"
#include "ObjectComponent/Rigidbody.h"

namespace ObjectComponent
{
    void Physics::initialize()
    {
        m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
        m_broadphase = std::make_unique<btDbvtBroadphase>();
        m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
            m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());
        m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    }

    void Physics::fixedUpdate(float timeStep)
    {
        for (Rigidbody* rigidbody : m_rigidbodies)
        {
            rigidbody->updateRigidbodyBeforeStep();
        }
        // Step the simulation
        m_dynamicsWorld->stepSimulation(timeStep, 10);

        // Sync all rigidbodies with their transforms
        for (Rigidbody* rigidbody : m_rigidbodies)
        {
            rigidbody->updateTransformAfterStep();
        }
    }

    void Physics::addRigidbody(Rigidbody* rigidbody)
    {
        m_dynamicsWorld->addRigidBody(rigidbody->getBulletRigidbody());
        m_rigidbodies.push_back(rigidbody);
    }

    void Physics::removeRigidbody(Rigidbody* rigidbody)
    {
        m_dynamicsWorld->removeRigidBody(rigidbody->getBulletRigidbody());
        auto it = std::find_if(m_rigidbodies.begin(), m_rigidbodies.end(),
            [rigidbody](Rigidbody* rb) { return rb == rigidbody; });

        if (it != m_rigidbodies.end())
        {
            m_rigidbodies.erase(it);
        }


    }
}