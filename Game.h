#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "UImanager.h"
#include "Dungeon.h"
#include "Player.h"
#include "NPC.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    UIManager* visualsManager;

    std::unique_ptr<Player> player;
    std::unique_ptr<Room> currentRoom;
    std::unique_ptr<Dungeon> dungeon;

    bool isPlayerInEncounter;
    int selectedPlayerAction;

    const Uint8* keyState;
    Uint32 lastMoveTime;
    static const Uint32 MOVE_DELAY = 90;

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