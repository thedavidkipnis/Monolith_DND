#pragma once
#include "NPCBehavior.h"
#include "NPC.h"

class BehaviorWander : public NPCBehavior {
public:
    void behave(NPC* npc, Room* room, int playerX, int playerY) override;
};