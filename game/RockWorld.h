#ifndef ROCKWORLD_H
#define ROCKWORLD_H

#include "World.h"

class RockWorld : public World {
public:
    RockWorld();

    void makeCurrent() override;
    void update(float dt) override;
};

#endif // ROCKWORLD_H
