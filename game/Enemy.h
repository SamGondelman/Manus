#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class CS123Shader;
class World;

class Enemy : public Entity {
public:
    Enemy() : Entity() {}
    Enemy(std::shared_ptr<btDiscreteDynamicsWorld> physWorld, ShapeType shapeType, btScalar mass,
          btVector3 pos, btVector3 scale, CS123SceneMaterial mat) : Entity(physWorld, shapeType, mass, pos, scale, mat) {}

    virtual void update(float dt, World &world) = 0;
    virtual void draw(std::shared_ptr<CS123Shader> program) { Entity::draw(); }
};

#endif // ENEMY_H
