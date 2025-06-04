#include "NPC.h"
#include "Constants.h"
#include <iostream>

NPC::NPC(int startX, int startY, int typeID) : x(startX), y(startY), typeID(typeID) {
    NPCFrame = { (startX * TILE_SIZE) + GAMEVIEW_START_X,
                 (startY * TILE_SIZE) + GAMEVIEW_START_Y,
                 TILE_SIZE,
                 TILE_SIZE };

};

NPC::~NPC() {
};

SDL_Rect& NPC::getNPCFrame() {
    return NPCFrame;
}

// Setters
void NPC::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
};

void NPC::setTypeID(int type) {
    typeID = type;
}