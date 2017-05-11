#include "RockWorld.h"

#include "glm.h"
#include "glm/gtx/transform.hpp"

#include "view.h"
#include "gl/shaders/CS123Shader.h"
#include "Player.h"
#include "CubeMesh.h"

RockWorld::RockWorld() : World(":/shaders/shader.vert", ":/shaders/shader.frag")
{

}

void RockWorld::makeCurrent() {
    m_lights.clear();
    m_lights.push_back(Light(glm::vec3(-1.0f), glm::vec3(0.7f)));

    World::makeCurrent();

    CS123SceneMaterial mat;
    mat.cAmbient = glm::vec4(0.125, 0.125, 0.05, 1);
    mat.cDiffuse = glm::vec4(0.25, 0.125, 0.15, 1);
    mat.cSpecular = glm::vec4(0.25, 0.25, 0.5, 1);
    mat.shininess = 10.0f;
    m_entities.emplace_back(m_physWorld, ShapeType::CUBE, 0.0f,
                            btVector3(0, -1, 0), btVector3(3.0, 0.25, 3.0), mat);
    m_entities.emplace_back(m_physWorld, ShapeType::CUBE, 0.0f,
                            btVector3(3, 0, 0), btVector3(0.25, 3.0, 3.0), mat);
}

void RockWorld::update(float dt) {
    World::update(dt);
    View::m_player->setEye(glm::vec3(0.0f, 1.5f + 0.5f * glm::sin(View::m_globalTime/3.0f), 6.0f));
    View::m_player->setCenter(glm::vec3(0));
}
