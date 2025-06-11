#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <algorithm>

struct PathNode {
    int x, y;
    int gCost, hCost;
    PathNode* parent;

    int fCost() const { return gCost + hCost; }

    bool operator==(const PathNode& other) const {
        return x == other.x && y == other.y;
    }
};

inline std::vector<std::pair<int, int>> findPath(Room* room, int startX, int startY, int targetX, int targetY) {
    const auto& tiles = room->getTiles();
    const int width = room->getWidth();
    const int height = room->getHeight();

    auto heuristic = [](int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2); // Manhattan distance
        };

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < width && y < height;
        };

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    std::vector<PathNode> openList;
    std::vector<std::vector<bool>> closed(height, std::vector<bool>(width, false));

    openList.push_back({ startX, startY, 0, heuristic(startX, startY, targetX, targetY), nullptr });

    while (!openList.empty()) {
        std::sort(openList.begin(), openList.end(), [](const PathNode& a, const PathNode& b) {
            return a.fCost() < b.fCost();
            });

        PathNode current = openList.front();
        openList.erase(openList.begin());
        closed[current.y][current.x] = true;

        // Stop if we're adjacent to the target (player)
        if (std::abs(current.x - targetX) + std::abs(current.y - targetY) == 1) {
            std::vector<std::pair<int, int>> path;
            for (PathNode* p = &current; p != nullptr; p = p->parent) {
                path.push_back({ p->x, p->y });
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& [dx, dy] : directions) {
            int nx = current.x + dx;
            int ny = current.y + dy;

            if (!inBounds(nx, ny) || !room->getTile(nx, ny)->getIsWalkable() || closed[ny][nx])
                continue;

            // Don't allow stepping onto the player's tile
            if (nx == targetX && ny == targetY)
                continue;

            int gCost = current.gCost + 1;
            int hCost = heuristic(nx, ny, targetX, targetY);

            bool inOpen = false;
            for (const auto& node : openList) {
                if (node.x == nx && node.y == ny && node.gCost <= gCost) {
                    inOpen = true;
                    break;
                }
            }

            if (!inOpen) {
                openList.push_back({ nx, ny, gCost, hCost, new PathNode(current) });
            }
        }
    }

    return {}; // No path found
}