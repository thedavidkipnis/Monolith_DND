#include "BehaviorAttackPlayerMelee.h"
#include "Constants.h"
#include <iostream> 

int BehaviorAttackPlayerMelee::behave(NPC* npc, Room* room, int playerX, int playerY) {

    int walkableDistance = npc->getMovementSpeed();
    int actionTaken = NPC_ACTION_NONE;

    for (int i = 0; i < walkableDistance; ++i) {
        int npcX = npc->getX();
        int npcY = npc->getY();

        // Check if already adjacent to the player — stop early
        if (std::abs(npcX - playerX) + std::abs(npcY - playerY) <= 1) {
            return NPC_ATTACK_PLAYER;
            break;
        }

        int dx = playerX - npcX;
        int dy = playerY - npcY;

        int stepX = 0;
        int stepY = 0;

        // Prioritize horizontal movement
        if (std::abs(dx) > std::abs(dy)) {
            stepX = (dx > 0) ? 1 : -1;
        }
        else if (dy != 0) {
            stepY = (dy > 0) ? 1 : -1; 
        }

        int nextX = npcX + stepX;
        int nextY = npcY + stepY;

        // Later we'll check if blocked
        npc->setPosition(nextX, nextY);
        actionTaken = NPC_MOVE;
    }    

    return actionTaken;
}