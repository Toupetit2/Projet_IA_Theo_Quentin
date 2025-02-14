#pragma once
#include "Enemy.hpp"
#include "ConditionNode.hpp"
#include "SequenceNode.hpp"
#include "SelectorNode.hpp"

class EnemyBehaviour : public Enemy {
public:
    EnemyBehaviour(float x, float y);

    void MovementManager();

    void update(float deltaTime, Grid& grid) override;
};