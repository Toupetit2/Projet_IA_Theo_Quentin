#include "Pathfinding2.hpp"
#include <queue>
#include <unordered_map>


#include <iostream>
class CompareNodePtr {
public:
    bool operator()(const Node2* a, const Node2* b) const {
        return a->fCost > b->fCost;
    }
};

struct Vector2iHash {
    std::size_t operator()(const sf::Vector2i& v) const {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};


std::vector<sf::Vector2i> Pathfinding2::findPath(Grid& grid, sf::Vector2i start, sf::Vector2i end) {
    std::priority_queue<Node2*, std::vector<Node2*>, CompareNodePtr> openQueue;
    std::unordered_map<sf::Vector2i, Node2*, Vector2iHash> allNodes;
    std::vector<sf::Vector2i> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {-1, -1}, {1, -1}, {1, 1}, {-1, 1}
    };

    Node2* startNode = new Node2(start);
    startNode->gCost = 0;
    startNode->hCost = startNode->calculateHeuristic(end);
    startNode->fCost = startNode->gCost + startNode->hCost;

    openQueue.push(startNode);
    allNodes[start] = startNode;

    while (!openQueue.empty()) {
        Node2* current = openQueue.top();
        openQueue.pop();

        if (current->position == end) {
            std::vector<sf::Vector2i> path;
            while (current) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());

            for (auto& pair : allNodes) delete pair.second;
            return path;
        }

        for (auto& dir : directions) {
            sf::Vector2i neighborPos = current->position + dir;

            if (neighborPos.x < 0 || neighborPos.x >= GRID_WIDTH || neighborPos.y < 0 || neighborPos.y >= GRID_HEIGHT)
                continue;
            if (!grid.getCell(neighborPos.y, neighborPos.x).walkable)
                continue;

            if ((dir.x != 0 && dir.y != 0) &&
                (!grid.getCell(current->position.y, neighborPos.x).walkable || !grid.getCell(neighborPos.y, current->position.x).walkable))
                continue;

            int newGCost = current->gCost + ((dir.x != 0 && dir.y != 0) ? 14 : 10); // 10 for straight, 14 for diagonal

            Node2* neighbor;
            if (allNodes.find(neighborPos) != allNodes.end()) {
                neighbor = allNodes[neighborPos];
                if (newGCost < neighbor->gCost) {
                    neighbor->gCost = newGCost;
                    neighbor->fCost = newGCost + neighbor->hCost;
                    neighbor->parent = current;
                    openQueue.push(neighbor);
                }
            }
            else {
                neighbor = new Node2(neighborPos);
                neighbor->gCost = newGCost;
                neighbor->hCost = neighbor->calculateHeuristic(end);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = current;
                openQueue.push(neighbor);
                allNodes[neighborPos] = neighbor;
            }
        }
    }

    for (auto& pair : allNodes) delete pair.second;
    return {};
}