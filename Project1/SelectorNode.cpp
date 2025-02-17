#include "SelectorNode.hpp"

void SelectorNode::AddChild(std::unique_ptr<BTNode> child)
{
    children.push_back(std::move(child));
}

NodeState SelectorNode::execute()
{
    for (auto& child : children) {
        if (child->execute() == NodeState::SUCCESS) {
            return NodeState::SUCCESS;
        }
    }
    return NodeState::FAILURE;
}
