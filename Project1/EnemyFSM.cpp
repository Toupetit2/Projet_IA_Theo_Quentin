#include "EnemyFSM.hpp"
#include "Pathfinding.hpp"

EnemyFSM::EnemyFSM(float x, float y) : Enemy(x, y) {}

void EnemyFSM::update(float deltaTime, Grid& grid, Entity& playerPos)
{
    switch (currentState) {
    case PATROL:
        Enemy::patrol(deltaTime);
        if (detectPlayer(playerPos.shape.getPosition()))
        { 
            currentState = CHASE;
        }
        break;

    case CHASE:
        if (!detectPlayer(playerPos.shape.getPosition()))
        {
            lastPlayerPos = playerPos.shape.getPosition();
            currentState = SEARCH;
        }
        Enemy::chase(playerPos.shape.getPosition(), deltaTime, grid);
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


void EnemyFSM::search(Vector2f lastPlayerPosition, float deltaTime, Grid& grid)
{
    Pathfinding pathfinding;
    float distanceBefore = std::sqrt((lastPlayerPos.x - shape.getPosition().x) * (lastPlayerPos.x - shape.getPosition().x) + (lastPlayerPos.y - shape.getPosition().y) * (lastPlayerPos.y - shape.getPosition().y));

    if (timeSinceSearchStarted == 0.0f || lastDirectionChangeTime > 1.f) {
        searchDirection = sf::Vector2f(rand() % 2 == 0 ? -1 : 1, rand() % 2 == 0 ? -1 : 1);
        searchDirection /= std::sqrt(searchDirection.x * searchDirection.x + searchDirection.y * searchDirection.y);
        lastDirectionChangeTime = 0;

    }

    timeSinceSearchStarted += deltaTime;
    lastDirectionChangeTime += deltaTime;
    if (timeSinceSearchStarted < searchTime) {
        shape.setPosition(shape.getPosition().x + (searchDirection.x * SPEED * deltaTime), shape.getPosition().y + (searchDirection.y * SPEED * deltaTime));
    }
    else {
        timeSinceSearchStarted = 0.0f;
        currentState = PATROL;
    }

    float distanceAfter = std::sqrt((lastPlayerPos.x - shape.getPosition().x) * (lastPlayerPos.x - shape.getPosition().x) + (lastPlayerPos.y - shape.getPosition().y) * (lastPlayerPos.y - shape.getPosition().y));
    if (distanceAfter > searchRange && distanceAfter > distanceBefore) {
        lastDirectionChangeTime = 10.0f;
    }
    if (collisionWithWall(grid))
    {
        cout << "collision !!!!!!!!!!!!!!!!!" << endl;
        shape.setPosition(shape.getPosition().x - (searchDirection.x * SPEED * deltaTime), shape.getPosition().y - (searchDirection.y * SPEED * deltaTime));
        lastDirectionChangeTime = 10.0f;
    }
}

bool EnemyFSM::collisionWithWall(Grid& grid)
{
    sf::FloatRect enemyBounds = shape.getGlobalBounds();

    // Calcul des indices de la grille où se trouve l'ennemi
    int left = enemyBounds.left / 40;
    int right = (enemyBounds.left + enemyBounds.width) / 40;
    int top = enemyBounds.top / 40;
    int bottom = (enemyBounds.top + enemyBounds.height) / 40;

    cout << left << " " << right << " " << top << " " << bottom << endl;

    if (!grid.getCell(top, left).walkable || !grid.getCell(top, right).walkable ||
        !grid.getCell(bottom, left).walkable || !grid.getCell(bottom, right).walkable) {
        return true; // Collision détectée
    }

    return false; // Pas de collision
}