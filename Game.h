#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>
#include "Constants.h"
#include "UImanager.h"
#include "Dungeon.h"
#include "Room.h"
#include "Player.h"
#include "Dungeon.h"
#include "MathConstants.h"
#include "NPC.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    UIManager* visualsManager;

    std::vector<Object*> RNGObjectDrops;

    std::unique_ptr<Player> player;
    std::vector<Object> playerInventory;
    Object* selectedInventoryItem;

    std::unique_ptr<Dungeon> dungeon;

    bool isPlayerInEncounter;
    int selectedPlayerAction;

    bool mapView;
    bool inventoryView;

    const Uint8* keyState;
    Uint32 lastMoveTime;
    static const Uint32 MOVE_DELAY = 90;

public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void cleanup();

private:
    void handleInput();
    void loadRNGObjectList();
    void playerPickUpObject(Object* obj);
    void processPlayerMove(int mouseX, int mouseY);
    void processPlayerAttack(int mouseX, int mouseY);
    void processPlayerInventoryUseItem();
    void processPlayerInventoryDropItem();
    void removePlayerInventoryItem(Object* obj);
    void processNPCLogic();
    void genRandomDrop(int x, int y);
    void update();
    void render();
};