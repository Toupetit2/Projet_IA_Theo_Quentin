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

    sf::CircleShape CirclePoint;

    sf::Vector2f firstPosition{ 2 * 40, 8 * 40 };
    sf::Vector2f secondPosition{ 7 * 40, 9 * 40 };
    sf::Vector2f thridPosition{ 7 * 40, 12 * 40 };
    sf::Vector2f fourthPosition{ 10 * 40, 11 * 40 };

public:

    enum State { PATROL, CHASE, SEARCH, ATTACK };

    State currentState;

    //void update(Grid& grid, sf::Vector2i target);

    EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start, int hp);

    bool detectPlayer(Entity& player);

    void playerDetected(Entity& player);

    void playerInRange(Entity& player);

    void playerLowLife();

    void patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint, Grid& grid) override;

    void update(float deltaTime, Grid& grid, Entity& player) override;

    void draw(sf::RenderWindow& window);
};