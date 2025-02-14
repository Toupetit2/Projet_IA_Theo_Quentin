#include "EnemyBehaviour.hpp"

EnemyBehaviour::EnemyBehaviour(float x, float y) : Enemy(x, y) {}

void EnemyBehaviour::update(float deltaTime, Grid& grid)
{
    Blackboard blackboard;
    blackboard.SetValue("PlayerDetected", 0); // ajout de nouvelles valeurs qui vont servir pour les conditions
    blackboard.SetValue("PlayerInRange", 0);
    blackboard.SetValue("PlayerLowLife", 0);


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