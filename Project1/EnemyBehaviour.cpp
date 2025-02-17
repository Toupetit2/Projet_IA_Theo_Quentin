#include "EnemyBehaviour.hpp"

EnemyBehaviour::EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start) : Enemy(x, y) {
    shape.setPosition(x, y);
    CircleDetect.setRadius(circleDetect);
    CircleDetect.setPosition(x, y);
    CircleDetect.setOrigin(circleDetect / 1.5, circleDetect / 1.5);
    CircleDetect.setFillColor(sf::Color(0, 255, 0, 50));
    CircleRange.setRadius(circleRange);
    CircleRange.setPosition(x, y);
    CircleRange.setOrigin(circleRange / 2, circleRange / 2);
    CircleRange.setFillColor(sf::Color(0, 0, 255, 50));
    blackboard.SetValue("PlayerDetected", 0);
    blackboard.SetValue("PlayerInRange", 0); // ajout de nouvelles valeurs qui vont servir pour les conditions
    blackboard.SetValue("PlayerLowLife", 0);
}

void EnemyBehaviour::PlayerDetected(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleDetect.getGlobalBounds())) {
        shape.setFillColor(sf::Color::Green);
        blackboard.SetValue("PlayerDetected", 1);
        currentState = CHASE;

        //shape.move();
    }
    else {
        blackboard.SetValue("PlayerDetected", 0);
        shape.setFillColor(sf::Color::Red);
    }
}

void EnemyBehaviour::PlayerInRange(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleRange.getGlobalBounds()) && player.shape.getGlobalBounds().intersects(CircleDetect.getGlobalBounds())) {
        shape.setFillColor(sf::Color::Blue);
        blackboard.SetValue("PlayerInRange", 1);
        blackboard.SetValue("PlayerDetected", 0);
    }
    else {
        blackboard.SetValue("PlayerInRange", 0);
        shape.setFillColor(sf::Color::Red);
    }
}

void EnemyBehaviour::PlayerLowLife()
{

}

void EnemyBehaviour::Patrolling()
{
    if (blackboard.GetValue("Patrouiller")) {
        static int currentWaypoint = 0;
        static sf::Vector2f waypoints[4] = { sf::Vector2f(100, 300), sf::Vector2f(500, 100), sf::Vector2f(100, 300), sf::Vector2f(500, 300) };
        sf::Vector2f target = waypoints[currentWaypoint];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f) {
            currentWaypoint = (currentWaypoint + 1) % 4;
        }
        else {
            direction /= distance;
            position += direction * 0.2f;
        }
        shape.setPosition(position);
    }
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

    sequence->AddChild(std::make_unique<ConditionNode>(blackboard, "PlayerLowLife", 1)); // si player low life == 1 alors fuit le joueur
    sequence->AddChild(std::make_unique<ActionNodeEscape>("Fuir"));

    selector->AddChild(std::move(sequenceSuivre)); // met la sequence suivre dans le selector
    selector->AddChild(std::move(sequenceAttaquer)); // met la sequence attaquer dans le selector

    root->AddChild(std::move(selector)); // met le selector dans le root
    root->AddChild(std::move(sequence)); // met la sequence dans le root / big boss

    root->AddChild(std::make_unique<ActionNodePatrol>("Patrouiller")); // si rien ne se passe alors on patrouille

    root->execute(); // execute le tout

    switch (currentState) {
    case PATROL:
        Patrolling();
        break;
        }
    /*case CHASE:
        chase(playerPos);
        if (!detectPlayer(playerPos)) {
            lastPlayerPosition = playerPos;
            currentState = SEARCH;
        }
        break;

    case SEARCH:
        search(lastPlayerPosition, deltaTime);
        break;
    }*/
}

void EnemyBehaviour::draw(sf::RenderWindow& window)
{
    window.draw(CircleDetect);
    window.draw(CircleRange);
    window.draw(shape);
}

/*void PNJ::chase(sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0) {
        direction /= distance;
        position += direction * 0.2f;
    }

    circle.setPosition(position);
}

void PNJ::search(sf::Vector2f lastPlayerPos, float deltaTime) {
    static float searchTimer = 0.0f;
    static sf::Vector2f searchDirection;

    if (searchTimer == 0.0f) {
        searchDirection = sf::Vector2f(rand() % 2 == 0 ? -1 : 1, rand() % 2 == 0 ? -1 : 1);
        searchDirection /= std::sqrt(searchDirection.x * searchDirection.x + searchDirection.y * searchDirection.y);
    }

    searchTimer += deltaTime;
    if (searchTimer < 10.0f) {
        position += searchDirection * 5.f * deltaTime;
    }
    else {
        searchTimer = 0.0f;
        currentState = PATROL;
    }

    float distance = std::sqrt((lastPlayerPos.x - position.x) * (lastPlayerPos.x - position.x) + (lastPlayerPos.y - position.y) * (lastPlayerPos.y - position.y));
    if (distance < detectionRadius) {
        searchTimer = 0.0f;
    }

    circle.setPosition(position);
}*/