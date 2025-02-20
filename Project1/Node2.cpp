#include "Node2.hpp"

Node2::Node2(sf::Vector2i pos)
    : position(pos), gCost(0), hCost(0), fCost(0), parent(nullptr) {
}

void Node2::calculateCosts(sf::Vector2i endPos, int newG) {
    gCost = newG;
    hCost = calculateHeuristic(endPos);
    fCost = gCost + hCost;
}

int Node2::calculateHeuristic(sf::Vector2i endPos) {
    int dx = std::abs(endPos.x - position.x);
    int dy = std::abs(endPos.y - position.y);

    return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
}