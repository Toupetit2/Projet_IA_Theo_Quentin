#include "EnemyBehaviour.hpp"

EnemyBehaviour::EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start, int hp) : Enemy(x, y, hp) {
    shape.setPosition(x, y);
    CircleDetect.setRadius(circleDetect);
    CircleDetect.setPosition(x, y);
    CircleDetect.setOrigin(180, 170);
    CircleDetect.setFillColor(sf::Color(0, 255, 0, 50));
    CircleRange.setRadius(circleRange);
    CircleRange.setPosition(x, y);
    CircleRange.setOrigin(30, 30);
    CircleRange.setFillColor(sf::Color(0, 0, 255, 50));
    blackboard.SetValue("PlayerDetected", 0);
    blackboard.SetValue("PlayerInRange", 0); // ajout de nouvelles valeurs qui vont servir pour les conditions
    blackboard.SetValue("PlayerLost", 0);
    CirclePoint.setRadius(20.f);
    CirclePoint.setFillColor(sf::Color::Magenta);
    currentState = PATROL;
}

bool EnemyBehaviour::detectPlayer(Entity& player) {

    float distance = std::sqrt(std::pow(player.shape.getPosition().x - shape.getPosition().x, 2) + std::pow(player.shape.getPosition().y - shape.getPosition().y, 2));
    return (distance < CircleDetect.getRadius());
}

void EnemyBehaviour::playerDetected(Entity& player)
{
    blackboard.SetValue("PlayerDetected", 1);
    currentState = CHASE;
    shape.setFillColor(sf::Color::Red);
}

void EnemyBehaviour::playerInRange(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleRange.getGlobalBounds()) && player.shape.getGlobalBounds().intersects(CircleDetect.getGlobalBounds())) {
        blackboard.SetValue("PlayerInRange", 1);
        blackboard.SetValue("PlayerDetected", 0);
        shape.setFillColor(sf::Color::White);
        //currentState = ATTACK;
    }
    else {
        blackboard.SetValue("PlayerInRange", 0);
    }
}

void EnemyBehaviour::playerLost()
{
    blackboard.SetValue("PlayerLost", 1);
    currentState = SEARCH;
    //shape.setFillColor(sf::Color::Green);
}

//void EnemyBehaviour::collisionWall(float deltaTime, Grid& grid, Vector2f direction)
//{
//    sf::Vector2f newPosition = shape.getPosition() + direction;
//    sf::FloatRect newBounds(newPosition, shape.getSize());
//
//    auto isWalkable = [&](float x, float y) {
//        int gridX = static_cast<int>(x / CELL_SIZE);
//        int gridY = static_cast<int>(y / CELL_SIZE);
//        return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).walkable;
//    };
//
//    if (isWalkable(newBounds.left - 1, newBounds.top - 1) &&
//        isWalkable(newBounds.left + newBounds.width + 1, newBounds.top - 1) &&
//        isWalkable(newBounds.left - 1, newBounds.top + newBounds.height + 1) &&
//        isWalkable(newBounds.left + newBounds.width + 1, newBounds.top + newBounds.height + 1)) {
//
//    }
//
//    if (!isWalkable(newBounds.left - 1, newBounds.top - 1)) {
//        cout << "left - 1 et top - 1" << endl;
//    }
//    else if (!isWalkable(newBounds.left + newBounds.width + 1, newBounds.top - 1)) {
//        cout << "left + width + 1 et top - 1" << endl;
//    }
//    else if (!isWalkable(newBounds.left - 1, newBounds.top + newBounds.height + 1)) {
//        cout << "left - 1 et top + height + 1" << endl;
//    }
//    else if (!isWalkable(newBounds.left + newBounds.width + 1, newBounds.top + newBounds.height + 1)) {
//        cout << "left + width + 1 et top + height + 1" << endl;
//    }
//    else if (!isWalkable(newBounds.left + newBounds.width + 1, newBounds.top + newBounds.height + 1)) {
//        cout << "left + width + 1 et top + height + 1" << endl;
//    }
//}

void EnemyBehaviour::patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint, Grid& grid)
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
        //cout << "numero de point : " << waypointCount << endl;
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
    CirclePoint.setPosition(waypoints[waypointCount]);
}

void EnemyBehaviour::search(float deltaTime, Grid& grid)
{
    Vector2f movement = Vector2f(0.f, 0.f);
    float distanceBefore = std::sqrt((lastPlayerPosition.x - shape.getPosition().x) * (lastPlayerPosition.x - shape.getPosition().x) + (lastPlayerPosition.y - shape.getPosition().y) * (lastPlayerPosition.y - shape.getPosition().y));

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

    float distanceAfter = std::sqrt((lastPlayerPosition.x - shape.getPosition().x) * (lastPlayerPosition.x - shape.getPosition().x) + (lastPlayerPosition.y - shape.getPosition().y) * (lastPlayerPosition.y - shape.getPosition().y));
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

bool EnemyBehaviour::collisionWithWall(Grid& grid)
{
    int left = shape.getPosition().x / 40;
    int right = (shape.getPosition().x + shape.getGlobalBounds().width) / 40;
    int top = shape.getPosition().y / 40;
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

void EnemyBehaviour::update(float deltaTime, Grid& grid, Entity& player)
{
    auto root = std::make_unique<SelectorNode>(); // notre racine (big boss)
    auto selector = std::make_unique<SelectorNode>(); // selecteur qui va gérer nos sequences suivre et attaque pour les transmettre au big boss
    auto sequence = std::make_unique<SequenceNode>(); // sequence de fuite
    auto sequenceSuivre = std::make_unique<SequenceNode>(); // sequence de suivre
    auto sequenceAttaquer = std::make_unique<SequenceNode>(); // sequence de attaquer

    sequenceSuivre->AddChild(std::make_unique<ConditionNode>(blackboard, "PlayerDetected", 1)); // si player detect == 1 alors suivre le joueur
    sequenceSuivre->AddChild(std::make_unique<ActionNodeChase>("Suivre le joueur"));

    sequenceAttaquer->AddChild(std::make_unique<ConditionNode>(blackboard, "PlayerInRange", 1)); // si player in range == 1 alors attaque le joueur
    sequenceAttaquer->AddChild(std::make_unique<ActionNodeAttack>("Attaquer"));

    sequence->AddChild(std::make_unique<ConditionNode>(blackboard, "PlayerLost", 1)); // si player low life == 1 alors fuit le joueur
    sequence->AddChild(std::make_unique<ActionNodeEscape>("Bah ? Il est ou ??"));

    selector->AddChild(std::move(sequenceSuivre)); // met la sequence suivre dans le selector
    selector->AddChild(std::move(sequenceAttaquer)); // met la sequence attaquer dans le selector

    root->AddChild(std::move(selector)); // met le selector dans le root
    root->AddChild(std::move(sequence)); // met la sequence dans le root / big boss

    root->AddChild(std::make_unique<ActionNodePatrol>("Patrouiller")); // si rien ne se passe alors on patrouille

    root->execute(); // execute le tout

    switch (currentState) {
    case PATROL:
        std::cout << "en patrouille" << std::endl;
        SPEED = 100.f;
        blackboard.SetValue("PlayerDetected", 0);
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("PlayerLost", 0);

        if (detectPlayer(player)) {
            playerDetected(player);
        }
        else {
            shape.setFillColor(sf::Color::Blue);
        }
        patrol(shape.getPosition(), deltaTime, firstPosition, secondPosition, thridPosition, fourthPosition, grid);
        break;
    
    case CHASE:
        std::cout << "en chasse" << std::endl;
        SPEED = 130.f;
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("PlayerLost", 0);

        if (!detectPlayer(player))
        {
            lastPlayerPosition = player.shape.getPosition();
            playerLost();
        }
        chase(player.shape.getPosition(), deltaTime, grid);
        shape.setFillColor(sf::Color::Red);
        playerInRange(player);
        break;

    case SEARCH:
        std::cout << "en recherche" << std::endl;
        SPEED = 100.f;
        blackboard.SetValue("PlayerDetected", 0);
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("PlayerLost", 1);

        if (!detectPlayer(player))
        {
            lastPlayerPosition = player.shape.getPosition();
            currentState = SEARCH;
        }
        shape.setFillColor(sf::Color::Green);
        search(deltaTime, grid);
        break;
    }

    CircleDetect.setPosition(shape.getPosition());
    CircleRange.setPosition(shape.getPosition());
}

void EnemyBehaviour::draw(sf::RenderWindow& window)
{
    window.draw(CircleDetect);
    window.draw(CircleRange);
    window.draw(shape);
    window.draw(CirclePoint);
}