#pragma once
#include "Enemy.hpp"
#include "ConditionNode.hpp"
#include "SequenceNode.hpp"
#include "SelectorNode.hpp"

class EnemyBehaviour : public Enemy {
protected : 
    Blackboard blackboard;
    sf::CircleShape CircleDetect;
    sf::CircleShape CircleRange;

public:
    EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange);

    void PlayerDetected(Entity& player);

    void PlayerInRange(Entity& player);

    void PlayerLowLife();

    void update(float deltaTime, Grid& grid) override;

    void draw(sf::RenderWindow& window);
};