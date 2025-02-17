#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) {}
void Enemy::update(float deltaTime, Grid& grid, Entity& playerPos) {}

void Enemy::chase(Vector2f pPos)
{
    sf::Vector2f direction = pPos - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 5.f) { // On se rapproche seulement si on est � plus de 5 pixels
        float angle = std::atan2(direction.y, direction.x); // Angle en radians
        shape.setPosition(shape.getPosition().x + (std::cos(angle) * SPEED), shape.getPosition().y + std::sin(angle) * SPEED);
    }
}

void Enemy::patrol()
{
    //shape.setPosition(pathfinding(shape.getPosition(), patrolTargetPositions[currentTargetID])[1]);
}

