#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>


#include <SFML/Graphics.hpp>
#include "Grid.hpp"

using namespace std;
using namespace sf;

class Entity {
public:
    RectangleShape shape;
    Vector2f velocity;
    int health;

    Entity(float x, float y, sf::Color color, int hp);
    virtual void update(float deltaTime, Grid& grid, Entity& entity) = 0;
    bool isAlive() const;
    void takeDamage(int damage);

};

#endif // ENTITY_HPP