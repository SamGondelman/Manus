#include "PhysicsWorld.h"

#include "view.h"
#include "gl/shaders/CS123Shader.h"
#include "Player.h"

#include "HangingEnemy.h"
#include "MarchingEnemy.h"

PhysicsWorld::PhysicsWorld() : World(":/shaders/shader.vert", ":/shaders/shader.frag")
{
}

void PhysicsWorld::makeCurrent() {
    View::m_player->setEye(glm::vec3(4, 2, 4));
    View::m_player->setCenter(glm::vec3(0));
    m_lights.clear();
    m_lights.emplace_back(glm::vec3(-1.0f), glm::vec3(0.7f));

    World::makeCurrent();

    CS123SceneMaterial mat;
    mat.cAmbient = glm::vec4(0.05, 0.1, 0, 1);
    mat.cDiffuse = glm::vec4(0.2, 0.6, 0.5, 1);
    mat.cSpecular = glm::vec4(0.5, 0.4, 0.7, 1);
    mat.shininess = 20.0f;
    m_entities.emplace_back(m_physWorld, ShapeType::SPHERE, 1.0f,
                            btVector3(1.5, 2, 0), btVector3(1.0, 1.0, 1.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CONE, 1.0f,
                            btVector3(-1.5, 2, 0), btVector3(1.0, 1.0, 1.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CUBE, 1.0f,
                            btVector3(0, 2, -1.5), btVector3(1.0, 1.0, 1.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CYLINDER, 1.0f,
                            btVector3(0, 2, 1.5), btVector3(1.0, 1.0, 1.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CUBE, 0.0f,
                            btVector3(0, -1, 0), btVector3(3.0, 0.5, 6.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CUBE, 0.0f,
                            btVector3(1.5, 1.75, 0), btVector3(0.5, 0.75, 2.0), mat);

    m_enemies.push_back(std::make_shared<HangingEnemy>(m_physWorld, btVector3(5, 3, -6), btVector3(0.25, 0.5, 0.25)));
    m_enemies.push_back(std::make_shared<HangingEnemy>(m_physWorld, btVector3(5, 2, -2), btVector3(0.25, 0.5, 0.25)));
    m_enemies.push_back(std::make_shared<HangingEnemy>(m_physWorld, btVector3(5, 3, 2), btVector3(0.25, 0.5, 0.25)));
    m_enemies.push_back(std::make_shared<HangingEnemy>(m_physWorld, btVector3(5, 2, 6), btVector3(0.25, 0.5, 0.25)));
    std::vector<glm::vec3> path1;
    path1.emplace_back(4, 0, 4);
    path1.emplace_back(-4, 0, 4);
    path1.emplace_back(-4, 0, -4);
    path1.emplace_back(4, 0, -4);
    m_enemies.push_back(std::make_shared<MarchingEnemy>(m_physWorld, btVector3(0.75, 0.75, 0.75), path1, 15.0f));
}

void PhysicsWorld::update(float dt) {
    World::update(dt);
}
