#ifndef MARCHINGENEMY_H
#define MARCHINGENEMY_H

#include "Enemy.h"

class MarchingEnemy : public Enemy {
public:
    MarchingEnemy() : Enemy() {}
    MarchingEnemy(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, btVector3 scale, std::vector<glm::vec3> &path, float duration) :
        Enemy(physWorld, ShapeType::SPHERE, 0.0f, btVector3(0, 0, 0), scale, getMat()), m_path(path), m_duration(duration) {}

    void update(float dt, World &world) override;

private:
    std::vector<glm::vec3> m_path;
    float m_duration;
    float m_bombTimer { 0.0f };

    CS123SceneMaterial getMat() {
        CS123SceneMaterial mat;
        mat.cAmbient.xyz = glm::vec3(0.1f);
        mat.cDiffuse.xyz = glm::vec3(1, 0.5, 0);
        mat.cSpecular.xyz = glm::vec3(0.5);
        mat.shininess = 20.0f;
        return mat;
    }
};

#endif // MARCHINGENEMY_H
