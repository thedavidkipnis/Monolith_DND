#include "Player.h"
#include "Room.h"
#include "Constants.h"

Player::Player() : x(12), y(9), speed(1), color(COLOR_BLUE) {
}

Player::Player(int startX, int startY) : x(startX), y(startY), speed(1), color(COLOR_BLUE) {
}

bool Player::tryMove(int deltaX, int deltaY, const Room& room) {
    int newX = x + deltaX * speed;
    int newY = y + deltaY * speed;

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

void Player::render(SDL_Renderer* renderer) const {
    SDL_Rect playerRect = {
        x * TILE_SIZE + 4,
        y * TILE_SIZE + 4,
        TILE_SIZE - 8,
        TILE_SIZE - 8
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &playerRect);
}