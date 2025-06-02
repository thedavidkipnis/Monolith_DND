#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class NPC {
private:
    int x, y;

public:
    NPC(int startX, int startY);
    ~NPC();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Movement
    void setPosition(int newX, int newY);
};