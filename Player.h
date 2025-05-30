#pragma once
#include <SDL.h>

class Room;

class Player {
private:
    int x, y;
    int speed;
    SDL_Color color;

public:
    Player();
    Player(int startX, int startY);

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getSpeed() const { return speed; }
    SDL_Color getColor() const { return color; }

    // Movement
    bool tryMove(int deltaX, int deltaY, const Room& room);
    void setPosition(int newX, int newY);

    // Rendering
    void render(SDL_Renderer* renderer) const;
};