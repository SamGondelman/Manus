#ifndef HANGINGENEMY_H
#define HANGINGENEMY_H

#include "Enemy.h"

class HangingEnemy : public Enemy {
public:
    HangingEnemy() : Enemy() {}
    HangingEnemy(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, btVector3 pos, btVector3 scale) :
        Enemy(physWorld, ShapeType::CUBE, 0.0f, pos, scale, getMat()) {}

    void update(float dt, std::shared_ptr<btDiscreteDynamicsWorld> physWorld) override;

private:
    glm::vec3 look { glm::vec3(0, -1, 0) };
    float m_angle { 0.0f };

    CS123SceneMaterial getMat() {
        CS123SceneMaterial mat;
        mat.cAmbient.xyz = glm::vec3(0.1f);
        mat.cDiffuse.xyz = glm::vec3(1, 0.5, 0);
        mat.cSpecular.xyz = glm::vec3(0.5);
        mat.shininess = 20.0f;
        return mat;
    }
};

#endif // HANGINGENEMY_H
