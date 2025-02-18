#include "EnemyFSM.hpp"
#include "Pathfinding.hpp"
#include "time.h"

EnemyFSM::EnemyFSM(float x, float y) : Enemy(x, y) {}

void EnemyFSM::update(float deltaTime, Grid& grid, Entity& player)
{
    switch (currentState) {
    case PATROL:
        Enemy::patrol(shape.getPosition(), deltaTime);
        if (detectPlayer(player.shape.getPosition()))
        { 
            currentState = CHASE;
        }
        break;

    case CHASE:
        if (!detectPlayer(player.shape.getPosition()))
        {
            lastPlayerPos = player.shape.getPosition();
            currentState = SEARCH;
        }

        Enemy::chase(player.shape.getPosition(), deltaTime, grid);
        break;

    case SEARCH:
        search(lastPlayerPos, deltaTime, grid);
        break;
    }
    
    if (lastState != currentState)
    {
        lastState = currentState;
        if (lastState == PATROL)
        {
            cout << "PATROL" << endl;
            shape.setFillColor(Color::Blue);
        }
        else if (lastState == CHASE)
        {
            cout << "CHASE" << endl;
            shape.setFillColor(Color::Red);
        }
        else if (lastState == SEARCH)
        {
            cout << "SEARCH" << endl;
            shape.setFillColor(Color::Green);
        }
        else 
        {
            cout << "ERROR - UNKNOWN STATE" << endl;
        }
    }
}

bool EnemyFSM::detectPlayer(Vector2f pPos)
{
    float dx = pPos.x - shape.getPosition().x;
    float dy = pPos.y - shape.getPosition().y;
    return dx * dx + dy * dy <= detectionRange * detectionRange;
}


void EnemyFSM::search(Vector2f lastPlayerPosition, float deltaTime, Grid& grid)
{
    Vector2f movement = Vector2f(0.f, 0.f);
    float distanceBefore = std::sqrt((lastPlayerPos.x - shape.getPosition().x) * (lastPlayerPos.x - shape.getPosition().x) + (lastPlayerPos.y - shape.getPosition().y) * (lastPlayerPos.y - shape.getPosition().y));

    if (timeSinceSearchStarted == 0.0f || lastDirectionChangeTime > 1.f) {
        searchDirection = sf::Vector2f(rand() % 2 == 0 ? -1 : 1, rand() % 2 == 0 ? -1 : 1);
        searchDirection /= std::sqrt(searchDirection.x * searchDirection.x + searchDirection.y * searchDirection.y);
        lastDirectionChangeTime = 0;

    }

    timeSinceSearchStarted += deltaTime;
    lastDirectionChangeTime += deltaTime;
    if (timeSinceSearchStarted < searchTime) {
        movement = Vector2f((searchDirection.x * SPEED * deltaTime), (searchDirection.y * SPEED * deltaTime));
    }
    else {
        timeSinceSearchStarted = 0.0f;
        currentState = PATROL;
    }

    float distanceAfter = std::sqrt((lastPlayerPos.x - shape.getPosition().x) * (lastPlayerPos.x - shape.getPosition().x) + (lastPlayerPos.y - shape.getPosition().y) * (lastPlayerPos.y - shape.getPosition().y));
    if (distanceAfter > searchRange && distanceAfter > distanceBefore) {
        lastDirectionChangeTime = 10.0f;
    }
    if (!collisionWithWall(grid))
    {
        shape.setPosition(shape.getPosition() + movement);
    }
    else
    {
        shape.setPosition(shape.getPosition() - movement);
        lastDirectionChangeTime = 10;
    }
}

bool EnemyFSM::collisionWithWall(Grid& grid)
{
    int left = shape.getPosition().x / 40;
    int right = (shape.getPosition().x + shape.getGlobalBounds().width) / 40;
    int top = shape.getPosition().y  / 40;
    int bottom = (shape.getPosition().y + shape.getGlobalBounds().height) / 40;

    try
    {
        if (left < 0 || right >= grid.getSize().x || top < 0 && bottom >= grid.getSize().y)
        {
            throw std::out_of_range("Les positions de l'ennemi sont en dehors de la grille. ");
        }
    }
    catch (const std::out_of_range& e)
    {
        cout << "Erreur : " << e.what() << endl;
    }

    if (!grid.getCell(left, top).walkable || !grid.getCell(right, top).walkable ||
        !grid.getCell(left, bottom).walkable || !grid.getCell(right, bottom).walkable) {
        return true;
    }

    return false;
}