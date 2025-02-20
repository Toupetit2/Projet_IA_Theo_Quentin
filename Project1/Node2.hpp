#ifndef NODE_HPP
#define NODE_HPP
#include <algorithm>
#include <SFML/System/Vector2.hpp>

class Node2 {
public:
    sf::Vector2i position;
    int gCost, hCost, fCost;
    Node2* parent;

    Node2(sf::Vector2i pos);
    void calculateCosts(sf::Vector2i endPos, int newG);
    int calculateHeuristic(sf::Vector2i endPos);
};

#endif