#include "BehaviorAttackPlayerMelee.h"
#include "Constants.h"
#include "MathConstants.h"
#include <iostream> 

int BehaviorAttackPlayerMelee::behave(NPC* npc, Room* room, int playerX, int playerY) {

    int npcX = npc->getX();
    int npcY = npc->getY();

    if (std::abs(npcX - playerX) + std::abs(npcY - playerY) <= 1) {
        return NPC_ATTACK_PLAYER;
    }

    auto path = findPath(room, npcX, npcY, playerX, playerY);

    // First step is the current tile — skip it
    if (path.size() > 1) {
        int steps = std::min(static_cast<int>(path.size()) - 1, npc->getMovementSpeed());
        auto [nextX, nextY] = path[steps];
        npc->setPosition(nextX, nextY);
        return NPC_MOVE;
    }

    return NPC_ACTION_NONE; // No path found
}