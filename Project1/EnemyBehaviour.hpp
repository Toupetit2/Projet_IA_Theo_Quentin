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
    sf::Vector2f lastPlayerPosition;
    int waypointCount = 0;

    sf::Vector2f firstPosition{ 100, 350 };
    sf::Vector2f secondPosition{ 200, 350 };
    sf::Vector2f thridPosition{ 300, 350 };
    sf::Vector2f fourthPosition{ 400, 350 };

public:
    enum StateWalkable { ONE, TWO, THREE, FOUR, FIVE };

    StateWalkable walkableState;

    enum State { PATROL, CHASE, SEARCH, ATTACK };

    State currentState;

    //void update(Grid& grid, sf::Vector2i target);

    EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start, int hp);

    bool detectPlayer(Entity& player);

    void playerDetected(Entity& player);

    void playerInRange(Entity& player);

    void playerLowLife();

    void patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint, Grid& grid);

    void update(float deltaTime, Grid& grid, Entity& player) override;

    void draw(sf::RenderWindow& window);
};