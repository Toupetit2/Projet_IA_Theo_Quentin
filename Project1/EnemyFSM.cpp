#include "EnemyFSM.hpp"

EnemyFSM::EnemyFSM(float x, float y) : Enemy(x, y) {}

void EnemyFSM::update(float deltaTime, Grid& grid, Entity& player)
{
    switch (currentState) {
    case PATROL:
        //Enemy::patrol(player.shape.getPosition());
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
        Enemy::chase(player.shape.getPosition(), deltaTime);
        break;

    case SEARCH:
        search(lastPlayerPos, deltaTime);
        break;
    }
    
    if (lastState != currentState)
    {
        lastState = currentState;
        if (lastState == PATROL)
        {
            cout << "PATROL" << endl;
        }
        else if (lastState == CHASE)
        {
            cout << "CHASE" << endl;
        }
        else if (lastState == SEARCH)
        {
            cout << "SEARCH" << endl;
        }
        else 
        {
            cout << "ERROR" << endl;
        }
    }

}

bool EnemyFSM::detectPlayer(Vector2f pPos)
{
    float dx = pPos.x - shape.getPosition().x;
    float dy = pPos.y - shape.getPosition().y;
    return dx * dx + dy * dy <= detectionRange * detectionRange;
}




void EnemyFSM::search(Vector2f lastPlayerPosition, float deltaTime)
{

    if (timeSinceSearchStarted == 0.0f) {
        searchDirection = sf::Vector2f(rand() % 2 == 0 ? -1 : 1, rand() % 2 == 0 ? -1 : 1);
        searchDirection /= std::sqrt(searchDirection.x * searchDirection.x + searchDirection.y * searchDirection.y);
    }

    timeSinceSearchStarted += deltaTime;
    if (timeSinceSearchStarted < searchTime) {
        shape.setPosition(shape.getPosition().x + (searchDirection.x * SPEED * deltaTime), shape.getPosition().y + (searchDirection.y * SPEED * deltaTime));
    }
    else {
        timeSinceSearchStarted = 0.0f;
        currentState = PATROL;
    }
}