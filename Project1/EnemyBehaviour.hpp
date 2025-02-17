#pragma once
#include "Enemy.hpp"
#include "Grid.hpp"
#include "ConditionNode.hpp"
#include "SequenceNode.hpp"
#include "SelectorNode.hpp"

class EnemyBehaviour : public Enemy {
protected : 
    Blackboard blackboard;
    sf::CircleShape CircleDetect;
    sf::CircleShape CircleRange;
    std::vector<sf::Vector2i> path;
    int pathIndex;
    sf::Clock moveClock;
    bool needsRepath;
    sf::Vector2f position;

public:

    sf::Vector2f lastPlayerPosition;
    enum State { PATROL, CHASE, SEARCH };
    State currentState;

    //void update(Grid& grid, sf::Vector2i target);

    EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start);

    void PlayerDetected(Entity& player);

    void PlayerInRange(Entity& player);

    void PlayerLowLife();

    void Patrolling();

    void update(float deltaTime, Grid& grid, Entity& player) override;

    void draw(sf::RenderWindow& window);
};