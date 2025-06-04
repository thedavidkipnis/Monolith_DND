#include "NPC.h"
#include "Constants.h"
#include <iostream>

NPC::NPC(int startX, int startY, int typeID) : 
    x(startX), 
    y(startY), 
    typeID(typeID) {

    switch (typeID) {
    case GOBLIN:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        break;
    case SPIDER:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        break;
    default:

        break;
    }

};

NPC::~NPC() {
};

// Setters
void NPC::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
};

void NPC::setTypeID(int type) {
    typeID = type;
}

void NPC::triggerBehavior(Room* room, int playerLocationX, int playerLocationY) {
    if (behavior) {
        behavior->behave(this, room, playerLocationX, playerLocationY);
    }
}