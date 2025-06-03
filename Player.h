#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Room.h"
#include <string>


class Player {
private:
    int x, y;
    SDL_Texture* spriteTexture = nullptr;

    int movement_speed;

public:
    Player();
    Player(int startX, int startY);
    ~Player();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    int getXTile() const;
    int getYTile() const;

    // Movement
    bool tryMove(int deltaX, int deltaY, const Room& room);
    void setPosition(int newX, int newY);

};