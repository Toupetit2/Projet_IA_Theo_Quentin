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

    Entity(float x, float y, sf::Color color);

    virtual void update(float deltaTime, Grid& grid, Entity& player) = 0;

};

#endif // ENTITY_HPP