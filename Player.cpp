#include "Player.h"
#include "Constants.h"
#include <iostream>

Player::Player() : x(12), y(9) {
}

Player::Player(int startX, int startY) : x(startX), y(startY) {
}

Player::~Player() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
    }
}

int Player::getXTile() const {
    return (x * TILE_SIZE) + GAMEVIEW_START_X;
};
int Player::getYTile() const {
    return (y * TILE_SIZE) + GAMEVIEW_START_Y;
};

bool Player::tryMove(int deltaX, int deltaY, const Room& room) {
    int newX = x + deltaX;
    int newY = y + deltaY;

    if (room.isWalkable(newX, newY)) {
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}