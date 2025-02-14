#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Grid.hpp"

using namespace sf;

class Entity {
public:
    RectangleShape shape;
    Vector2f velocity;

    Entity(float x, float y, sf::Color color);
    virtual void update(float deltaTime, Grid& grid, Vector2f playerPos) = 0;
};

#endif // ENTITY_HPP