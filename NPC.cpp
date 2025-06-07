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
        healthPoints = 5;
        movementSpeed = 3;
        break;
    case SPIDER:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        healthPoints = 2;
        movementSpeed = 6;
        break;
    default:
        healthPoints = 0;
        movementSpeed = 0;
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

void NPC::setMovementSpeed(int ms) {
    movementSpeed = ms;
}


void NPC::triggerBehavior(Room* room, int playerLocationX, int playerLocationY) {
    if (behavior) {
        behavior->behave(this, room, playerLocationX, playerLocationY);
    }
}