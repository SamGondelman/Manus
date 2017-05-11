#include "HangingEnemy.h"

#include "view.h"
#include "Player.h"

#include <iostream>
void HangingEnemy::update(float dt) {
    glm::vec3 p = View::m_player->getEye();
    glm::mat4 m;
    getModelMatrix(m);
    glm::vec3 e = glm::vec3(m[3][0], m[3][1], m[3][2]);
    glm::vec3 l1 = glm::normalize(p - e);
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
