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
    mapView(false),
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
    player = std::make_unique<Player>(12, 9, 5, 8, 1, 3);

    dungeon = std::make_unique<Dungeon>();
    dungeon->generateFloorRooms(15);

    isPlayerInEncounter = false;

    keyState = SDL_GetKeyboardState(nullptr);
    running = true;
    return true;
}

void Game::handleInput() {

    Room* curRoom = dungeon->getCurrentRoom();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        // for quitting out of the death screen
        if (player->getHealthPoints() < 1) {
            if (e.type == SDL_KEYDOWN && e.button.button == SDL_SCANCODE_SPACE) {
                running = false;

            }
        }

        // this is for skipping turn to trigger properly
        if (e.type == SDL_KEYUP && e.button.button == SDL_SCANCODE_SPACE) {
            selectedPlayerAction = END_TURN;
            visualsManager->setUITextboxText("ENDING TURN...");

        }

        if (e.type == SDL_KEYUP && e.button.button == SDL_SCANCODE_M) {
            mapView = false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            
            // if click was inside of a UI panel, let UIManager handle it
            if ((e.button.x < GAMEVIEW_START_X || e.button.x > GAMEVIEW_START_X + GAMEVIEW_WIDTH)
                || (e.button.y < GAMEVIEW_START_Y || e.button.y > GAMEVIEW_START_Y + GAMEVIEW_HEIGHT)) {
                switch (visualsManager->checkUIButtonPress(e.button.x, e.button.y)) {
                    case ATTACK:
                        selectedPlayerAction = ATTACK;
                        visualsManager->setUITextboxText("SELECTED ATTACK");
                        break;
                    case END_TURN:
                        selectedPlayerAction = END_TURN;
                        visualsManager->setUITextboxText("ENDING TURN...");
                        break;
                    case MOVE:
                        selectedPlayerAction = MOVE;
                        visualsManager->setUITextboxText("SELECTED MOVE");
                        break;
                    default:
                        selectedPlayerAction = NONE;
                        visualsManager->setUITextboxText("");
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
                        //player->attack(mouseX, mouseY);
                        curRoom->processPlayerAttack(mouseX, mouseY, player->getDamage());
                    }
                    else {
                        visualsManager->setUITextboxText("TOO FAR TO ATTACK");
                    }
                    break;
                case END_TURN: // SHOULD NEVER BE AN OPTION
                    std::cout << "Attempting END TURN\n";
                    break;
                case MOVE:
                    if (curRoom->isWalkableTurnBased(player->getX(), player->getY(), mouseX, mouseY, player->getMovementSpeedLeft())) {
                        player->makeMoveTurnBased(mouseX, mouseY);
                    }
                    else {
                        if (findDistanceInTiles(mouseX, mouseY, player->getX(), player->getY()) > player->getMovementSpeedLeft()) {
                            visualsManager->setUITextboxText("NOT ENOUGHT MOVEMENT SPEED LEFT");
                        }
                        else {
                            visualsManager->setUITextboxText("INVALID LOCATION");
                        }
                    }
                    break;
                default: // when user clicks on gameview with no selected action

                    if (dungeon->getCurrentRoom()->getTile(mouseX, mouseY)->getIsOccupied()) {
                        visualsManager->setFocusedNPC(curRoom->getNPCAt(mouseX, mouseY));
                        visualsManager->setUITextboxText(curRoom->getNPCAt(mouseX, mouseY)->getDescription());
                    }
                    else {
                        visualsManager->setUITextboxText(curRoom->getTile(mouseX, mouseY)->getTileDescription());
                    }
                    break;
                }

                SDL_Point mousePoint = { mouseX, mouseY };              
            }
        }
    }

    // Handle continuous key presses with timing
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > MOVE_DELAY) {

        if (keyState[SDL_SCANCODE_M]) {
            mapView = true;
        }

        // see if user is trying to switch actions
        if (keyState[SDL_SCANCODE_2]) {
            selectedPlayerAction = MOVE;
            visualsManager->setUITextboxText("SELECTED MOVE");
        }
        if (keyState[SDL_SCANCODE_3]) {
            selectedPlayerAction = ATTACK;
            visualsManager->setUITextboxText("SELECTED ATTACK");
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
                if (curRoom && curRoom->isWalkable(deltaX + player->getX(), deltaY + player->getY())) {
                    player->setPosition(deltaX + player->getX(), deltaY + player->getY());
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

    if (player->getHealthPoints() < 1) {
        // you died game over!
    }

    if(!curRoom->hasRoomBeenVisited())
    {
        if (curRoom->isRoomEncounter()) {
            visualsManager->setUITextboxText(
                "ENCOUNTERED " + std::to_string(curRoom->getListOfNPCs()->size()) + " ENEMIES!"
            );
        }
        else {
            visualsManager->setUITextboxText("NOTHING HERE...");
        }
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

        processNPCLogic();
    }

}

void Game::movePlayerAndUpdateRoomTileOccupiedStatus(int oldX, int oldY, int newX, int newY) {
    player->makeMoveTurnBased(newX, newY);
    dungeon->getCurrentRoom()->getTile(oldX, oldY)->setIsOccupied(false);
    dungeon->getCurrentRoom()->getTile(newX, newY)->setIsOccupied(true);
}

void Game::processNPCLogic() {

    Room* currentRoom = dungeon->getCurrentRoom();
    std::vector<NPC*>* npcs = currentRoom->getListOfNPCs();

    int actionTaken = NPC_ACTION_NONE;
    std::string NPCActionDisplayString = "";

    for (NPC* npc : *npcs)
    {
        int oldNpcX = npc->getX();
        int oldNpcY = npc->getY();
        actionTaken = npc->triggerBehavior(currentRoom, player->getX(), player->getY());

        if (NPCActionDisplayString.size() > 0) {
            NPCActionDisplayString += "\n";
        }

        switch (actionTaken) {
        case NPC_ATTACK_PLAYER:
            NPCActionDisplayString += "NPC ATTACKED PLAYER WITH " + std::to_string(npc->getDamage()) + " DAMAGE";
            player->setHealthPoints(player->getHealthPoints() - npc->getDamage());
            break;
        case NPC_MOVE:
            NPCActionDisplayString += "NPC MOVED";

            currentRoom->getTile(oldNpcX, oldNpcY)->setIsOccupied(false);
            currentRoom->getTile(npc->getX(), npc->getY())->setIsOccupied(true);

            break;
        default:
            NPCActionDisplayString += "NO NPC ACTION TAKEN";
            break;
        }
    }
    visualsManager->setUITextboxText(NPCActionDisplayString);
}

void Game::render() {

    if (player->getHealthPoints() < 1) { // DEAD
        visualsManager->renderDeathScreen();
    }
    else {
        if (mapView) {
            visualsManager->renderMap(dungeon->getRooms(), dungeon->getCurRoomCoord(), mapView);
        }
        else {
            Room* currentRoom = dungeon->getCurrentRoom();
            visualsManager->render(currentRoom, player.get(), selectedPlayerAction);
            visualsManager->renderMap(dungeon->getRooms(), dungeon->getCurRoomCoord(), mapView);
        } 
    }

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