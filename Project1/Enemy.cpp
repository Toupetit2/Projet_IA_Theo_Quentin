#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) {}
void Enemy::update(float deltaTime, Grid& grid, Entity& playerPos) {}

void Enemy::chase(Vector2f pPos, float deltaTime, Grid& grid)
{
    Pathfinding pathfinding;
    vector<Vector2i> path = pathfinding.findPath(grid, Vector2i(shape.getPosition().x/40, shape.getPosition().y / 40), Vector2i(pPos.x/40, pPos.y/40));
    
    if (path.size() > 1) { // On se rapproche seulement si on est � plus de 5 pixels
        sf::Vector2f direction = Vector2f(path[1]*40) - shape.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float angle = std::atan2(direction.y, direction.x); // Angle en radians
        shape.setPosition(shape.getPosition().x + (std::cos(angle) * SPEED * deltaTime), shape.getPosition().y + std::sin(angle) * SPEED * deltaTime);
    }
}

void Enemy::patrol(float deltaTime)
{
    //shape.setPosition(pathfinding(shape.getPosition(), patrolTargetPositions[currentTargetID])[1]);
}

