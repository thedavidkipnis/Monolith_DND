#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Room.h"
#include <string>


class Player {
private:
    int x, y;
    SDL_Texture* spriteTexture = nullptr;

    int movementSpeed;
    int movementSpeedLeft;

    int attackRange;
    int damage;

public:
    Player();
    Player(int startX, int startY, int movementSpeed, int attackRange, int damage);
    ~Player();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    int getXTile() const;
    int getYTile() const;

    // Movement
    bool tryMove(int deltaX, int deltaY, const Room& room);
    void setPosition(int newX, int newY);

    int getMovementSpeed() const { return movementSpeed; }
    int getMovementSpeedLeft() const { return movementSpeedLeft; }

    void setMovementSpeed(int sp);
    void setMovementSpeedLeft(int sp);

    bool tryMoveTurnBased(int destX, int destY, const Room& room);
    void makeMoveTurnBased(int mouseX, int mouseY);

    bool tryAttacking(int destX, int destY);
    void attack(int destX, int destY); 

};