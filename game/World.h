#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "CS123SceneData.h"
#include "Entity.h"
#include "Enemy.h"

class CS123Shader;

class World {
public:
    World() {}
    World(std::string vert, std::string frag);
    virtual ~World();

    virtual void makeCurrent();
    virtual void update(float dt);
    virtual void drawGeometry();

    std::vector<Light>& getLights() { return m_lights; }
    std::shared_ptr<CS123Shader> getWorldProgram() { return m_program; }
    std::shared_ptr<btDiscreteDynamicsWorld> getPhysWorld() { return m_physWorld; }
    std::vector<Entity>& getEntities() { return m_entities; }

protected:
    std::shared_ptr<CS123Shader> m_program;
    std::vector<Light> m_lights;

    // Physics stuff
    std::unique_ptr<btDefaultCollisionConfiguration> m_configuration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::shared_ptr<btDiscreteDynamicsWorld> m_physWorld;

    // TODO: figure out how to make this a std::vector<std::shared_ptr<Entity>>
    std::vector<Entity> m_entities;
    std::vector<std::shared_ptr<Enemy>> m_enemies;
};

#endif // WORLD_H
