#include "EnemyFSM.hpp"
#include "Pathfinding.hpp"

EnemyFSM::EnemyFSM(float x, float y, int hp) : Enemy(x, y, hp) {}

void EnemyFSM::update(float deltaTime, Grid& grid, Entity& player)
{
    switch (currentState) {
    case PATROL:
        patrol(shape.getPosition(), deltaTime, firstPosition, secondPosition, thridPosition, fourthPosition);
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

        chase(player.shape.getPosition(), deltaTime, grid);
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
        //cout << "collision !!!!!!!!!!!!!!!!!" << endl;
        shape.setPosition(shape.getPosition().x - (searchDirection.x * SPEED * deltaTime), shape.getPosition().y - (searchDirection.y * SPEED * deltaTime));
        lastDirectionChangeTime = 10.0f;
    }
}

bool EnemyFSM::collisionWithWall(Grid& grid)
{
    sf::FloatRect enemyBounds = shape.getGlobalBounds();

    // Calcul des indices de la grille o� se trouve l'ennemi
    int left = enemyBounds.left / 40;
    int right = (enemyBounds.left + enemyBounds.width) / 40;
    int top = enemyBounds.top / 40;
    int bottom = (enemyBounds.top + enemyBounds.height) / 40;

    //cout << left << " " << right << " " << top << " " << bottom << endl;

    if (!grid.getCell(top, left).walkable || !grid.getCell(top, right).walkable ||
        !grid.getCell(bottom, left).walkable || !grid.getCell(bottom, right).walkable) {
        return true; // Collision d�tect�e
    }

    return false; // Pas de collision
}

void EnemyFSM::patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint)
{
    static int currentWaypoint = 0;
    static sf::Vector2f waypoints[4] = { sf::Vector2f(firstPoint), sf::Vector2f(secondPoint), sf::Vector2f(thirdPoint), sf::Vector2f(fourthPoint) };
    sf::Vector2f target = waypoints[currentWaypoint];
    sf::Vector2f direction = target - ePos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentWaypoint = (currentWaypoint + 1) % 4;
    }
    else {
        direction /= distance;
        ePos += direction * deltaTime * SPEED;
    }
    shape.setPosition(ePos);
}