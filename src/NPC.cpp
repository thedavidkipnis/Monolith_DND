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
        movementSpeed = 4;
        damage = 2;
        description = "A PESKY GOBLIN";
        textureID = "goblin";
        displayTextureID = "goblin_display";
        name = "GOBLIN";
        break;
    case SPIDER:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        healthPoints = 2;
        movementSpeed = 6;
        damage = 1;
        description = "AN OVERSIZED ARACHNID";
        textureID = "spider";
        displayTextureID = "spider_display";
        name = "SPIDER";
        break;
    case BANDIT:
        behavior = std::make_unique<BehaviorAttackPlayerMelee>();
        healthPoints = 3;
        movementSpeed = 3;
        damage = 2;
        description = "A MISCREANT";
        textureID = "bandit_" + std::to_string(getRandomIntInRange(1, 2));
        displayTextureID = "bandit_display";
        name = "BANDIT";
        break;
    default:
        healthPoints = 0;
        movementSpeed = 0;
        damage = 0;
        description = "MISSING ENTITY DESCRIPTION";
        textureID = "none";
        displayTextureID = "none";
        name = "NONE";
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


int NPC::triggerBehavior(Room* room, int playerLocationX, int playerLocationY) {
    if (behavior) {
        return behavior->behave(this, room, playerLocationX, playerLocationY);
    }
    return NPC_ACTION_NONE;
}