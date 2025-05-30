#include "Game.h"
#include "Player.h"
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

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
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

    // Initialize game objects
    player = std::make_unique<Player>(12, 9);
    dungeon = std::make_unique<Dungeon>();
    dungeon->generateInitialRooms();

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
    // Future: enemy AI, item interactions, room transitions, etc.
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    // Render game objects
    Room* currentRoom = dungeon->getCurrentRoom();
    if (currentRoom) {
        currentRoom->render(renderer);
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

    SDL_Quit();
}