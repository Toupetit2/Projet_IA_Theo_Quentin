#pragma once
#include "Enemy.hpp"

class EnemyBehaviour : public Enemy {
public:
    EnemyBehaviour(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};