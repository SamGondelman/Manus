#include "HangingEnemy.h"

#include "view.h"
#include "Player.h"

#include "World.h"
#include "CylinderMesh.h"
#include "gl/shaders/CS123Shader.h"

void HangingEnemy::update(float dt, World &world) {
    glm::mat4 m;
    getModelMatrix(m);
    glm::vec3 e = glm::vec3(m[3][0], m[3][1], m[3][2]);
    glm::vec3 p = View::m_player->getEye();
    glm::vec3 l1 = glm::normalize(p - e);

    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(e.x, e.y, e.z), btVector3(p.x, p.y, p.z));
    world.getPhysWorld()->rayTest(btVector3(e.x, e.y, e.z), btVector3(p.x, p.y, p.z), RayCallback);

    if (glm::acos(glm::dot(l1, look)) < M_PI_4 && !RayCallback.hasHit()) {
        // Pointed towards player and not blocked
        m_canSee = true;
        getMaterial().cDiffuse = glm::vec4(1, 0, 0, 1);
        getMaterial().cAmbient = glm::vec4(0.1, 0, 0, 1);
    } else {
        // Not pointed towards player or blocked
        m_canSee = false;
        m_angle += dt;
        l1 = glm::quat(glm::vec3(-glm::radians(35.0 + 15.0f * glm::cos(m_angle/3.5f)), m_angle/5.0f, 0)) * glm::vec3(0, 0, -1);
        getMaterial().cDiffuse = glm::vec4(0, 1, 0, 1);
        getMaterial().cAmbient = glm::vec4(0, 0.1, 0, 1);
    }
    // TODO: slerp instead for smoother motion
    look = glm::normalize(0.99f * look + 0.01f * l1);

    float yaw = atan2(look.x, look.z);
    float pitch = M_PI_2 - asin(look.y);
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(e.x, e.y, e.z));
    t.setRotation(btQuaternion(yaw, pitch, 0));
    m_rigidBody->setWorldTransform(t);
    m_rigidBody->getMotionState()->setWorldTransform(t);
}

void HangingEnemy::draw(std::shared_ptr<CS123Shader> program) {
    Enemy::draw(program);

    // Laser
    if (m_canSee) {
        CS123SceneMaterial mat;
        mat.cDiffuse = glm::vec4(10, 10, 0, 1);
        mat.cAmbient = glm::vec4(15, 0, 0, 1);
        mat.cSpecular = glm::vec4(5, 5, 5, 1);
        mat.shininess = 100.0f;

        glm::mat4 m;
        getModelMatrix(m);
        glm::vec3 e = glm::vec3(m[3][0], m[3][1], m[3][2]);
        glm::vec3 p = View::m_player->getEye();
        float dist = glm::length(p - e);
        glm::vec3 d = dist * look;
        float yaw = atan2(d.x, d.z);
        float pitch = M_PI_2 - asin(d.y/dist);
        glm::mat4 r = glm::mat4_cast(glm::quat(glm::vec3(pitch, yaw, 0.0f)));
        m = glm::translate(e + d/2.0f) * r * glm::scale(glm::vec3(0.02, dist, 0.02));
        program->setUniform("M", m);
        program->applyMaterial(mat);
        View::m_cylinder->draw();
    }
}
