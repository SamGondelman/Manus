#include "MarchingEnemy.h"

#include "view.h"
#include "World.h"

void MarchingEnemy::update(float dt, World &world) {
    float f = glm::fract(View::m_globalTime / m_duration);
    float fi = f * m_path.size();
    glm::vec3 e = glm::mix(m_path[(int) fi], m_path[(((int) fi) + 1) % m_path.size()], glm::fract(fi));

    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(e.x, e.y, e.z));
    m_rigidBody->setWorldTransform(t);
    m_rigidBody->getMotionState()->setWorldTransform(t);

    const float BOMB_TIME = 3.0f;
    m_bombTimer += dt;
    if (m_bombTimer > BOMB_TIME) {
        CS123SceneMaterial mat;
        mat.cAmbient = glm::vec4(0, 0.01f, 0.05f, 1);
        mat.cDiffuse = glm::vec4(0, 0.1f, 0.5f, 1);
        mat.cSpecular = glm::vec4(0.3f, 0.1f, 0.5f, 1);
        mat.shininess = 0.5f;
        const float VEL_MAG = 10.0f;
        glm::vec3 vel = VEL_MAG * (glm::quat(glm::vec3(M_PI_4, View::m_globalTime, 0)) * glm::vec3(0, 0, -1));
        world.getEntities().emplace_back(world.getPhysWorld(), ShapeType::SPHERE, 1.0f,
                                         btVector3(e.x, e.y, e.z), btVector3(0.2f, 0.2f, 0.2f),
                                         mat, btQuaternion(0, 0, 0, 1), btVector3(vel.x, vel.y, vel.z));
        m_bombTimer = 0.0f;
    }
}
