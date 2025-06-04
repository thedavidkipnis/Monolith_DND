#include "Game.h"
#include "Player.h"
#include "NPC.h"
#include "Dungeon.h"
#include "Room.h"
#include "Constants.h"
#include <iostream>

Game::Game()
    : window(nullptr), 
    renderer(nullptr), 
    visualsManager(nullptr), 
    running(false), 
    isPlayerInEncounter(false), 
    selectedPlayerAction(NONE),
    lastMoveTime(0),
    keyState(nullptr) {
}

Game::~Game() {
    cleanup();
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

    visualsManager = new UIManager(renderer);

    // Initialize game objects
    player = std::make_unique<Player>(12, 9, 5, 1, 1);

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

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            
            // if click was inside of a UI panel, let UIManager handle it
            if ((e.button.x < GAMEVIEW_START_X || e.button.x > GAMEVIEW_START_X + GAMEVIEW_WIDTH)
                || (e.button.y < GAMEVIEW_START_Y || e.button.y > GAMEVIEW_START_Y + GAMEVIEW_HEIGHT)) {
                switch (visualsManager->checkUIButtonPress(e.button.x, e.button.y)) {
                    case ATTACK:
                        std::cout << "Selected ATTACK.\n";
                        selectedPlayerAction = ATTACK;
                        break;
                    case END_TURN:
                        std::cout << "Ending turn...\n";
                        selectedPlayerAction = END_TURN;
                        break;
                    case MOVE:
                        std::cout << "Selected MOVE.\n";
                        selectedPlayerAction = MOVE;
                        break;
                    default:
                        std::cout << "Outside game view. No button press.\n";
                        selectedPlayerAction = NONE;
                        break;
                }
            }
            // otherwise, player clicked somewhere on GameView
            else
            {

                // adjusting click coordinates to gameview screen
                int mouseX = (e.button.x - (e.button.x % TILE_SIZE) - GAMEVIEW_START_X) / TILE_SIZE;
                int mouseY = (e.button.y - (e.button.y % TILE_SIZE) - GAMEVIEW_START_Y) / TILE_SIZE;

                switch (selectedPlayerAction) {
                case ATTACK:
                    if (player->tryAttacking(mouseX, mouseY)) {
                        player->attack(mouseX, mouseY);
                        dungeon->getCurrentRoom()->processPlayerAttack(mouseX, mouseY);
                    }
                    else {
                        std::cout << "Can't attack: ";
                        std::cout << mouseX;
                        std::cout << " ";
                        std::cout << mouseY;
                        std::cout << ". Too far.\n";
                    }
                    break;
                case END_TURN: // SHOULD NEVER BE AN OPTION
                    std::cout << "Attempting END TURN\n";
                    break;
                case MOVE:
                    if (player->tryMoveTurnBased(mouseX, mouseY, *dungeon->getCurrentRoom())) {
                        player->makeMoveTurnBased(mouseX, mouseY);
                    }
                    else {
                        std::cout << "Not enough movement speed to move to: ";
                        std::cout << mouseX;
                        std::cout << " ";
                        std::cout << mouseY;
                        std::cout << "\n";
                    }
                    break;
                default:
                    // NO ACTION
                    // COULD ADD TILE INSPECT FUNCTIONALITY HERE
                    std::cout << findDistanceInTiles(mouseX, mouseY, player->getX(), player->getY()) << "\n";
                    break;
                }

                SDL_Point mousePoint = { mouseX, mouseY };              
            }
        }
    }

    // Handle continuous key presses with timing
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > MOVE_DELAY) {

        // see if user is trying to switch actions
        if (keyState[SDL_SCANCODE_1]) {
            selectedPlayerAction = END_TURN;
        }
        if (keyState[SDL_SCANCODE_2]) {
            selectedPlayerAction = MOVE;
        }
        if (keyState[SDL_SCANCODE_3]) {
            selectedPlayerAction = ATTACK;
        }


        if (!isPlayerInEncounter) {
            int deltaX = 0, deltaY = 0;

            if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
                deltaY = -1;
            }
            if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
                deltaY = 1;
            }
            if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
                deltaX = -1;
                player->setWhichDirectionIsFacing(LEFT);
            }
            if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
                deltaX = 1;
                player->setWhichDirectionIsFacing(RIGHT);
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
}

void Game::update() {
    // Game logic updates would go here
    // Future: enemy AI, item interactions

    Room* curRoom = dungeon->getCurrentRoom();

    if(!curRoom->hasRoomBeenVisited())
    {
        if (curRoom->isRoomEncounter()) {
            std::cout << "entered encounter room. ";
        }
        else {
            std::cout << "not an encounter. ";
        }
        std::cout << "room has ";
        std::cout << curRoom->getListOfNPCs()->size();
        std::cout << " NPC(s).\n";

        curRoom->setRoomVisited(true);
    }

    if (curRoom->getListOfNPCs()->size() > 0) { // if we encounter enemies
        isPlayerInEncounter = true;
    }
    else { // when there are no more enemies
        curRoom->setRoomEncounterState(false);
        isPlayerInEncounter = false;
        player->setMovementSpeedLeft(player->getMovementSpeed());
    }

    if (selectedPlayerAction == END_TURN) {
        player->setMovementSpeedLeft(player->getMovementSpeed());
        selectedPlayerAction = NONE;

        dungeon->getCurrentRoom()->processNPCActions(player->getX(), player->getY());
    }

}

void Game::render() {

    // Render game objects
    Room* currentRoom = dungeon->getCurrentRoom();

    visualsManager->render(currentRoom, player.get());

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

    if (visualsManager) {
        visualsManager->~UIManager();
    }

    SDL_Quit();
}