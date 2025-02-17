#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) {}
void Enemy::update(float deltaTime, Grid& grid, Entity& player) {}


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

void Enemy::patrol(Vector2f ePos, float deltaTime)

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
        ePos += direction * SPEED * deltaTime;
        cout << ePos.x << ", " << ePos.y << endl;
        
    }
    shape.setPosition(ePos);
}