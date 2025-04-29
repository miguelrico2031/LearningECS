#pragma once
#include <memory>
#include <btBulletDynamicsCommon.h>
#include <vector>

namespace ObjectComponent
{
    class Rigidbody;

    class Physics
    {
    public:
        Physics() {}
        //~Physics() {
        //    m_dynamicsWorld.reset();
        //    m_solver.reset();
        //    m_broadphase.reset();
        //    m_dispatcher.reset();
        //    m_collisionConfiguration.reset();
        //}

        void initialize();
        void fixedUpdate();
        void addRigidbody(Rigidbody* rigidbody);
        void removeRigidbody(Rigidbody* rigidbody);

    private:
        std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> m_dispatcher;
        std::unique_ptr<btBroadphaseInterface> m_broadphase;
        std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
        std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

        std::vector<Rigidbody*> m_rigidbodies;
    };
}