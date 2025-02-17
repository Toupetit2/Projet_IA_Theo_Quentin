#pragma once
#include "BTNode.hpp"
#include "Blackboard.hpp"

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    int expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, int value);
    NodeState execute() override;
};

class ActionNodePatrol : public BTNode {
private:
    std::string actionName;
public:
    ActionNodePatrol(std::string name);
    NodeState execute() override;
};

class ActionNodeChase : public BTNode {
private:
    std::string actionName;
public:
    ActionNodeChase(std::string name);
    NodeState execute() override;
};

class ActionNodeAttack : public BTNode {
private:
    std::string actionName;
public:
    ActionNodeAttack(std::string name);
    NodeState execute() override;
};

class ActionNodeEscape : public BTNode {
private:
    std::string actionName;
public:
    ActionNodeEscape(std::string name);
    NodeState execute() override;
};