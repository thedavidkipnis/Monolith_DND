#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class NPC {
private:
    int x, y;
    SDL_Texture* spriteTexture = nullptr;

public:
    NPC(int startX, int startY);
    ~NPC();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Movement
    void setPosition(int newX, int newY);

    // Rendering
    void render(SDL_Renderer* renderer) const;
    bool loadTexture(SDL_Renderer* renderer, const std::string& filePath);
};