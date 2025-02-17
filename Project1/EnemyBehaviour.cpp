#include "EnemyBehaviour.hpp"

EnemyBehaviour::EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start) : Enemy(x, y) {
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
    blackboard.SetValue("PlayerLowLife", 0);
    currentState = PATROL;
}

bool EnemyBehaviour::detectPlayer(Entity& player) {

    float distance = std::sqrt(std::pow(player.shape.getPosition().x - shape.getPosition().x, 2) + std::pow(player.shape.getPosition().y - shape.getPosition().y, 2));
    return (distance < CircleDetect.getRadius());
}

void EnemyBehaviour::PlayerDetected(Entity& player)
{
    shape.setFillColor(sf::Color::Green);
    blackboard.SetValue("PlayerDetected", 1);
    currentState = CHASE;
}

void EnemyBehaviour::PlayerLowLife()
{

}

void EnemyBehaviour::PlayerInRange(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleRange.getGlobalBounds()) && player.shape.getGlobalBounds().intersects(CircleDetect.getGlobalBounds())) {
        shape.setFillColor(sf::Color::Blue);
        blackboard.SetValue("PlayerInRange", 1);
        blackboard.SetValue("PlayerDetected", 0);
        //currentState = ATTACK;
    }
    else {
        blackboard.SetValue("PlayerInRange", 0);
        shape.setFillColor(sf::Color::Red);
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

    sequence->AddChild(std::make_unique<ConditionNode>(blackboard, "LowLife", 1)); // si player low life == 1 alors fuit le joueur
    sequence->AddChild(std::make_unique<ActionNodeEscape>("Fuir"));

    selector->AddChild(std::move(sequenceSuivre)); // met la sequence suivre dans le selector
    selector->AddChild(std::move(sequenceAttaquer)); // met la sequence attaquer dans le selector

    root->AddChild(std::move(selector)); // met le selector dans le root
    root->AddChild(std::move(sequence)); // met la sequence dans le root / big boss

    root->AddChild(std::make_unique<ActionNodePatrol>("Patrouiller")); // si rien ne se passe alors on patrouille

    root->execute(); // execute le tout

    switch (currentState) {
    case PATROL:
        std::cout << "en patrouille" << std::endl;
        blackboard.SetValue("PlayerDetected", 0);
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("LowLife", 0);

        if (detectPlayer(player)) {
            PlayerDetected(player);
        }
        patrol(shape.getPosition(), deltaTime);
        break;
    
    case CHASE:
        std::cout << "en chasse" << std::endl;
        SPEED = 90.f;
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("LowLife", 0);

        if (!detectPlayer(player))
        {
            lastPlayerPosition = player.shape.getPosition();
            currentState = PATROL;
        }
        chase(player.shape.getPosition(), deltaTime, grid);
        PlayerInRange(player);
        break;

    case SEARCH:
        std::cout << "en recherche" << std::endl;

        blackboard.SetValue("PlayerDetected", 0);
        blackboard.SetValue("PlayerInRange", 0);
        blackboard.SetValue("LowLife", 0);

        //search(lastPlayerPosition, deltaTime);
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
}