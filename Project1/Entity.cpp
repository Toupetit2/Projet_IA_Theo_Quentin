#include "Entity.hpp"

Entity::Entity(float x, float y, sf::Color color, int hp) : health(hp) {
    shape.setSize({ 35, 35 });
    shape.setPosition(x, y);
    shape.setFillColor(color);
}

bool Entity::isAlive() const {
    return health > 0;
}

void Entity::takeDamage(int damage) {
    health -= damage;
}