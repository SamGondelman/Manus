#include "HangingEnemy.h"

#include "view.h"
#include "Player.h"

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

void HangingEnemy::update(float dt, std::shared_ptr<btDiscreteDynamicsWorld> physWorld) {
    glm::mat4 m;
    getModelMatrix(m);
    glm::vec3 e = glm::vec3(m[3][0], m[3][1], m[3][2]);
    glm::vec3 p = View::m_player->getEye();
    glm::vec3 l1 = glm::normalize(p - e);

    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(e.x, e.y, e.z), btVector3(p.x, p.y, p.z));
    physWorld->rayTest(btVector3(e.x, e.y, e.z), btVector3(p.x, p.y, p.z), RayCallback);

    if (glm::acos(glm::dot(l1, look)) < M_PI_4 && !RayCallback.hasHit()) {
        // Pointed towards player and not blocked
        getMaterial().cDiffuse = glm::vec4(1, 0, 0, 1);
        getMaterial().cAmbient = glm::vec4(0.1, 0, 0, 1);
    } else {
        // Not pointed towards player or blocked
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
