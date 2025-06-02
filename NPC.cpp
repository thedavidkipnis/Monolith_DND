#include "NPC.h"
#include "Constants.h"
#include <iostream>

NPC::NPC(int startX, int startY) : x(startX), y(startY) {
};

NPC::~NPC() {
};

// Setters
void NPC::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
};