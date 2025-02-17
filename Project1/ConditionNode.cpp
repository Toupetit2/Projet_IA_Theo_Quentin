#include "ConditionNode.hpp"

ConditionNode::ConditionNode(Blackboard& bb, const std::string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}

NodeState ConditionNode::execute()
{
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

// ACTION 

ActionNodePatrol::ActionNodePatrol(std::string name) : actionName(name) {}

NodeState ActionNodePatrol::execute()
{
    std::cout << "Action de patrol: " << actionName << std::endl;
    return NodeState::SUCCESS;
}

ActionNodeChase::ActionNodeChase(std::string name) : actionName(name) {}

NodeState ActionNodeChase::execute()
{
    std::cout << "Action de chase: " << actionName << std::endl;
    // bool follow = true donc on fait appel au movement manager du pnj pour lui permettre de se déplacer
    return NodeState::SUCCESS;
}

ActionNodeAttack::ActionNodeAttack(std::string name) : actionName(name) {}

NodeState ActionNodeAttack::execute()
{
    std::cout << "Action d'attaque: " << actionName << std::endl;
    return NodeState::SUCCESS;
}

ActionNodeEscape::ActionNodeEscape(std::string name) : actionName(name) {}

NodeState ActionNodeEscape::execute()
{
    std::cout << "Action: " << actionName << std::endl;
    return NodeState::SUCCESS;
}