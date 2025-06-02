#include "Player.h"
#include "Constants.h"
#include <iostream>

Player::Player() : x(12), y(9), speed(1), color(COLOR_BLUE) {
}

Player::Player(int startX, int startY) : x(startX), y(startY), speed(1), color(COLOR_BLUE) {
}

Player::~Player() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
    }
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

bool Player::loadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return false;
    }

    spriteTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!spriteTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_Rect destRect = {
        (x * TILE_SIZE) + GAMEVIEW_START_X,
        (y * TILE_SIZE) + GAMEVIEW_START_Y,
        TILE_SIZE,
        TILE_SIZE
    };

    if (spriteTexture) {
        SDL_RenderCopy(renderer, spriteTexture, nullptr, &destRect);
    }
    else {
        // Fallback to colored square if texture isn't loaded
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &destRect);
    }
}