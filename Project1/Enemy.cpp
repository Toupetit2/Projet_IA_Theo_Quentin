#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) {}
void Enemy::update(float deltaTime, Grid& grid, Entity& player) {}

void Enemy::chase(Vector2f pPos, float deltaTime)
{
    sf::Vector2f direction = pPos - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 5.f) { // On se rapproche seulement si on est � plus de 5 pixels
        float angle = std::atan2(direction.y, direction.x); // Angle en radians
        shape.setPosition(shape.getPosition().x + (std::cos(angle) * SPEED), shape.getPosition().y + std::sin(angle) * SPEED);
    }
}

void Enemy::patrol(Vector2f ePos)
{
    static int currentWaypoint = 0;
    static sf::Vector2f waypoints[4] = { sf::Vector2f(300, 150), sf::Vector2f(500, 500), sf::Vector2f(150, 300), sf::Vector2f(500, 300) };
    sf::Vector2f target = waypoints[currentWaypoint];
    sf::Vector2f direction = target - ePos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentWaypoint = (currentWaypoint + 1) % 4;
    }
    else {
        direction /= distance;
        ePos += direction * 1.f;
    }
    shape.setPosition(ePos);
}