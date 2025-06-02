#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "Dungeon.h"
#include "Player.h"
#include "NPC.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    SDL_Rect gameView;
    SDL_Rect leftUIPanel;
    SDL_Rect rightUIPanel;
    SDL_Rect bottomUIPanel;

    bool isPlayerInEncounter;

    std::unique_ptr<Player> player;
    std::unique_ptr<Room> currentRoom;
    std::unique_ptr<Dungeon> dungeon;

    SDL_Texture* wallTexture = nullptr;
    SDL_Texture* doorTexture = nullptr;
    SDL_Texture* NPCTexture = nullptr;

    const Uint8* keyState;
    Uint32 lastMoveTime;
    static const Uint32 MOVE_DELAY = 150;

public:
    Game();
    ~Game();

    void loadTextures();
    bool initialize();
    void run();
    void cleanup();

    SDL_Renderer* getRenderer() const { return renderer; }

private:
    void handleInput();
    void update();
    void renderUIPanel(SDL_Renderer* renderer, SDL_Rect panel);
    void render();
};