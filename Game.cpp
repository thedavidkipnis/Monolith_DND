#include "Game.h"
#include "Player.h"
#include "NPC.h"
#include "Dungeon.h"
#include "Room.h"
#include "Constants.h"
#include <iostream>

Game::Game()
    : window(nullptr), renderer(nullptr), running(false), lastMoveTime(0) {
}

Game::~Game() {
    cleanup();
}

void Game::loadTextures() {
    SDL_Surface* surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/wall.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!wallTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/wood_door.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    doorTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!doorTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/goblin.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    NPCTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!NPCTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("Roguelike Dungeon Crawler",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gameView = { UI_SIDE_PANEL_WIDTH, 0, GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT };
    leftUIPanel = { 0, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    rightUIPanel = { UI_SIDE_PANEL_WIDTH + GAMEVIEW_WIDTH, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    bottomUIPanel = { 0, GAMEVIEW_HEIGHT, UI_BOTTOM_PANNEL_WIDTH, UI_BOTTOM_PANNEL_HEIGHT };


    loadTextures();

    // Initialize game objects
    player = std::make_unique<Player>(12, 9);
    player->loadTexture(renderer, "C:/Users/theda/source/repos/Monolith_DND/rogue.png");

    isPlayerInEncounter = false;

    dungeon = std::make_unique<Dungeon>();
    dungeon->generateInitialRooms();

    isPlayerInEncounter = false;

    keyState = SDL_GetKeyboardState(nullptr);
    running = true;
    return true;
}

void Game::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
    }

    // Handle continuous key presses with timing
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > MOVE_DELAY) {
        int deltaX = 0, deltaY = 0;

        if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
            deltaY = -1;
        }
        if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
            deltaY = 1;
        }
        if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
            deltaX = -1;
        }
        if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
            deltaX = 1;
        }

        // Try to move player
        if (deltaX != 0 || deltaY != 0) {
            Room* currentRoom = dungeon->getCurrentRoom();
            if (currentRoom && player->tryMove(deltaX, deltaY, *currentRoom)) {
                lastMoveTime = currentTime;

                // Check for room transitions
                int newPlayerX, newPlayerY;
                if (dungeon->tryRoomTransition(player->getX(), player->getY(), newPlayerX, newPlayerY)) {
                    player->setPosition(newPlayerX, newPlayerY);
                }
            }
        }
    }
}

void Game::update() {
    // Game logic updates would go here
    // Future: enemy AI, item interactions

    Room* curRoom = dungeon->getCurrentRoom();

    if(!curRoom->hasRoomBeenVisited())
    {
        if (curRoom->isRoomEncounter()) {
            std::cout << "entered encounter room. ";
            curRoom->setRoomEncounterState(false);
            curRoom->setRoomVisited(true);
        }
        else {
            std::cout << "not an encounter. ";
            curRoom->setRoomVisited(true);
        }
        std::cout << "room has ";
        std::cout << curRoom->getListOfNPCs()->size();
        std::cout << " enemies.\n";
    }
}

void Game::renderUIPanel(SDL_Renderer* renderer, SDL_Rect panel) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &panel);
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    renderUIPanel(renderer, leftUIPanel);
    renderUIPanel(renderer, rightUIPanel);
    renderUIPanel(renderer, bottomUIPanel);

    // Render game objects
    Room* currentRoom = dungeon->getCurrentRoom();
    if (currentRoom) {
        currentRoom->render(renderer, wallTexture, doorTexture);
        currentRoom->renderRoomNPCs(renderer, NPCTexture);
    }
    player->render(renderer);

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void Game::run() {
    while (running) {
        handleInput();
        update();
        render();

        // Cap the frame rate (roughly 60 FPS)
        SDL_Delay(16);
    }
}

void Game::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (wallTexture) {
        SDL_DestroyTexture(wallTexture);
    }

    SDL_Quit();
}