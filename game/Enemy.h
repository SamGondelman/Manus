#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy() : Entity() {}
    Enemy(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, ShapeType shapeType, btScalar mass,
          btVector3 pos, btVector3 scale, CS123SceneMaterial mat) : Entity(physWorld, shapeType, mass, pos, scale, mat) {}

    virtual void update(float dt, std::shared_ptr<btDiscreteDynamicsWorld> physWorld) = 0;
};

#endif // ENEMY_H
