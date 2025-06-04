#pragma once

class NPC;
class Room;

class NPCBehavior {
public:
    virtual ~NPCBehavior() = default;

    // This will be implemented differently by each behavior type
    virtual void behave(NPC* npc, Room* room, int playerX, int playerY) = 0;
};