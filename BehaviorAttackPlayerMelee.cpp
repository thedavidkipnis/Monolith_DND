#include "BehaviorAttackPlayerMelee.h"
#include <iostream> 

void BehaviorAttackPlayerMelee::behave(NPC* npc, Room* room, int playerX, int playerY) {
    int npcX = npc->getX();
    int npcY = npc->getY();

    int dx = playerX - npcX;
    int dy = playerY - npcY;

    int stepX = 0;
    int stepY = 0;

    // Prioritize horizontal movement first (or you can prioritize vertical if you'd prefer)
    if (std::abs(dx) > std::abs(dy)) {
        stepX = (dx > 0) ? 1 : -1;
    }
    else if (dy != 0) {
        stepY = (dy > 0) ? 1 : -1;
    }

    // You might want to check for collisions here (e.g., with walls or other NPCs)
    npc->setPosition(npcX + stepX, npcY + stepY);
}