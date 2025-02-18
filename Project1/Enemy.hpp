#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Pathfinding.hpp"
#include "Entity.hpp"

class Enemy : public Entity {
private:

public:
    float SPEED = 100.f;

    Enemy(float x, float y, int hp);

    void update(float deltaTime, Grid& grid, Entity& player) override;

    void chase(Vector2f pPos, float deltaTime, Grid& grid);
    virtual void patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint);

    //void search(float deltaTime);

};

#endif // ENEMY_HPP