#include "NPC.h"
#include "Constants.h"
#include <iostream>

NPC::NPC(int startX, int startY, int typeID) : 
    x(startX), 
    y(startY), 
    typeID(typeID),
    healthPoints(healthPoints) {

    switch (typeID) {
    case GOBLIN:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        healthPoints = 5;
        break;
    case SPIDER:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        healthPoints = 2;
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

void NPC::setHealthPoints(int hp) {
    healthPoints = hp;
}


void NPC::triggerBehavior(Room* room, int playerLocationX, int playerLocationY) {
    if (behavior) {
        behavior->behave(this, room, playerLocationX, playerLocationY);
    }
}