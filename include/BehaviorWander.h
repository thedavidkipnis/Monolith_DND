#pragma once
#include "NPCBehavior.h"
#include "NPC.h"

class BehaviorWander : public NPCBehavior {
public:
    int behave(NPC* npc, Room* room, int playerX, int playerY) override;
};