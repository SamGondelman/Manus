#include "World.h"

#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"

World::World(std::string vert, std::string frag)
{
    std::string vertexSource = ResourceLoader::loadResourceFileToString(vert);
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(frag);
    m_program = std::make_shared<CS123Shader>(vertexSource, fragmentSource);

    m_configuration = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher = std::make_unique<btCollisionDispatcher>(m_configuration.get());
    m_broadphase = std::make_unique<btDbvtBroadphase>();
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_physWorld = std::make_shared<btDiscreteDynamicsWorld>(m_dispatcher.get(),
                                                            m_broadphase.get(),
                                                            m_solver.get(),
                                                            m_configuration.get());
    m_physWorld->setGravity(btVector3(0,-9.81,0));
}

void World::update(float dt) {
    m_physWorld->stepSimulation(dt);
}

void World::makeCurrent() {
    if (m_entities.size() > 0) {
        for (auto& e : m_entities) m_physWorld->removeRigidBody(e.m_rigidBody.get());
        m_entities.clear();
        m_solver->reset();
        m_physWorld->clearForces();
        m_broadphase->resetPool(m_dispatcher.get());
    }
}
