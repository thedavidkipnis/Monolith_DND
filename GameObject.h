#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class GameObject {
private:
    int x, y;

public:
    GameObject(int startX, int startY);
    ~GameObject();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Movement
    void setPosition(int newX, int newY);
};