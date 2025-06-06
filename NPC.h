#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "NPCBehavior.h"
#include "BehaviorWander.h"
#include "BehaviorAttackPLayerMelee.h"
#include <memory>

class NPC {
private:
    int x, y;
    int typeID;

    int healthPoints;

    std::unique_ptr<NPCBehavior> behavior;

public:
    NPC(int startX, int startY, int typeID);
    ~NPC();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    int getTypeID() const { return typeID; }
    void setTypeID(int type);

    int getHealthPoints() const { return healthPoints; }
    void setHealthPoints(int hp);

    // Movement
    void setPosition(int newX, int newY);

    // Logic
    void triggerBehavior(Room* room, int playerLocationX, int playerLocationY);
};