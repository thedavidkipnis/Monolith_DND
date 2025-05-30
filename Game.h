#pragma once
#include <SDL.h>
#include <memory>
#include "Dungeon.h"

class Player;
class Room;

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    std::unique_ptr<Player> player;
    std::unique_ptr<Room> currentRoom;
    std::unique_ptr<Dungeon> dungeon;

    const Uint8* keyState;
    Uint32 lastMoveTime;
    static const Uint32 MOVE_DELAY = 150;

public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void cleanup();

    SDL_Renderer* getRenderer() const { return renderer; }

private:
    void handleInput();
    void update();
    void render();
};