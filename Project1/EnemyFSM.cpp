#include "EnemyFSM.hpp"
#include "Pathfinding2.hpp"
#include "time.h"

EnemyFSM::EnemyFSM(float x, float y, int hp) : Enemy(x, y, hp) {
    shape.setOutlineColor(Color::Blue);
    shape.setOutlineThickness(3.f);
}

void EnemyFSM::update(float deltaTime, Grid& grid, Entity& player)
{
    switch (currentState) {
    case PATROL:
        patrol(shape.getPosition(), deltaTime, firstPosition, secondPosition, thridPosition, fourthPosition, grid);

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
        search(lastPlayerPos, deltaTime, grid, player);
        break;
    }
    
    if (lastState != currentState)
    {
        lastState = currentState;
        if (lastState == PATROL)
        {
            cout << "PATROL" << endl;
            shape.setOutlineColor(Color::Blue);
        }
        else if (lastState == CHASE)
        {
            cout << "CHASE" << endl;
            shape.setOutlineColor(Color::Red);
        }
        else if (lastState == SEARCH)
        {
            cout << "SEARCH" << endl;
            shape.setOutlineColor(Color::Green);
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


void EnemyFSM::search(Vector2f lastPlayerPosition, float deltaTime, Grid& grid, Entity& player)
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

    if (detectPlayer(player.shape.getPosition())) {
        currentState = CHASE;
        timeSinceSearchStarted = 0.0f;
        lastDirectionChangeTime = 0;
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
    return false; // Pas de collision
}

void EnemyFSM::patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint, Grid& grid)
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

    Pathfinding pathfinding;
    vector<Vector2i> path = pathfinding.findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(waypoints[waypointCount].x / 40, waypoints[waypointCount].y / 40));

    if ((shape.getPosition().x) / 40 < (waypoints[waypointCount].x + 50) / 40 && (shape.getPosition().x) / 40 > (waypoints[waypointCount].x - 50) / 40
        && (shape.getPosition().y) / 40 < (waypoints[waypointCount].y + 50) / 40 && (shape.getPosition().y) / 40 > (waypoints[waypointCount].y - 50) / 40) {
        waypointCount += 1;
        if (waypointCount > 3) {
            waypointCount = 0;
        }
    }

    if (path.size() > 1) { // On se rapproche seulement si on est a plus de 5 pixels
        sf::Vector2f direction = Vector2f(path[1] * 40) - shape.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float angle = std::atan2(direction.y, direction.x); // Angle en radians
        shape.setPosition(shape.getPosition().x + (std::cos(angle) * SPEED * deltaTime), shape.getPosition().y + std::sin(angle) * SPEED * deltaTime);
    }
}