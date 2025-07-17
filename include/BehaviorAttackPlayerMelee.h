#pragma once
#include "NPCBehavior.h"
#include "NPC.h"
#include "Room.h"

class BehaviorAttackPlayerMelee : public NPCBehavior {
public:
    int behave(NPC* npc, Room* room, int playerX, int playerY) override;
};
