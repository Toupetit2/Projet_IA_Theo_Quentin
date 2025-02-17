#include "EnemyBehaviour.hpp"

EnemyBehaviour::EnemyBehaviour(std::string n, float x, float y, float circleDetect, float circleRange) : Enemy(x, y) {
    shape.setPosition(x, y);
    CircleDetect.setRadius(circleDetect);
    CircleDetect.setPosition(x, y);
    CircleDetect.setOrigin(35, 35);
    CircleDetect.setFillColor(sf::Color(100, 255, 0, 50));
    CircleRange.setRadius(circleRange);
    CircleRange.setPosition(x, y);
    CircleRange.setOrigin(35, 35);
    CircleDetect.setFillColor(sf::Color(0, 255, 0, 50));
    blackboard.SetValue("PlayerDetected", 0);
    blackboard.SetValue("PlayerInRange", 0); // ajout de nouvelles valeurs qui vont servir pour les conditions
    blackboard.SetValue("PlayerLowLife", 0);
}

void EnemyBehaviour::PlayerDetected(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleDetect.getGlobalBounds())) {
        shape.setFillColor(sf::Color::Green);
        blackboard.SetValue("PlayerDetected", 1);
    }
}

void EnemyBehaviour::PlayerInRange(Entity& player)
{
    if (player.shape.getGlobalBounds().intersects(CircleRange.getGlobalBounds())) {
        shape.setFillColor(sf::Color::Blue);
        blackboard.SetValue("PlayerInRange", 1);
    }
}

void EnemyBehaviour::PlayerLowLife()
{

}

void EnemyBehaviour::update(float deltaTime, Grid& grid)
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
}

void EnemyBehaviour::draw(sf::RenderWindow& window)
{
    window.draw(CircleDetect);
    window.draw(CircleRange);
    window.draw(shape);
}
