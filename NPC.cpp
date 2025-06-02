#include "NPC.h"
#include "Constants.h"
#include <iostream>

NPC::NPC(int startX, int startY) : x(startX), y(startY) {
};

NPC::~NPC() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
    }
};

// Setters
void NPC::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
};

// Rendering
void NPC::render(SDL_Renderer* renderer) const {
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
        SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
        SDL_RenderFillRect(renderer, &destRect);
    }
};
bool NPC::loadTexture(SDL_Renderer* renderer, const std::string& filePath) {
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
};