#include "Game.h"

Game::Game()
    : window(nullptr), 
    renderer(nullptr), 
    visualsManager(nullptr), 
    running(false), 
    isPlayerInEncounter(false), 
    selectedPlayerAction(NONE),
    lastMoveTime(0),
    mapView(false),
    inventoryView(false),
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

    loadRNGObjectList();

    // Initialize game objects
    player = std::make_unique<Player>(12, 9, 5, 8, 10, 0, 1, 1, 1, 3);
    selectedInventoryItem = nullptr;

    Object* potion1 = new Object(14, 9, 1, true, "A WARM ELIXIR", "HEALTH POTION", "health_potion");
    Object* potion2 = new Object(10, 9, 1, true, "AN ENERGIZING ELIXIR", "ENERGY POTION", "energy_potion");

    dungeon = std::make_unique<Dungeon>();
    dungeon->generateFloorRooms(15);

    isPlayerInEncounter = false;

    dungeon->getCurrentRoom()->addObjectToRoom(potion1);
    dungeon->getCurrentRoom()->addObjectToRoom(potion2);


    keyState = SDL_GetKeyboardState(nullptr);
    running = true;
    return true;
}

void Game::loadRNGObjectList() {
    RNGObjectDrops.push_back(new Object(-1, -1, 1, true, "A BIT O' WEALTH", "GOLD COINS", "coins"));
    RNGObjectDrops.push_back(new Object(-1, -1, 1, true, "A WARM ELIXIR", "HEALTH POTION", "health_potion"));
    RNGObjectDrops.push_back(new Object(-1, -1, 1, true, "AN ENERGIZING ELIXIR", "ENERGY POTION", "energy_potion"));
    RNGObjectDrops.push_back(new Object(-1, -1, 1, true, "A HEARTY MEAL", "STIR FRY", "stir_fry"));
    RNGObjectDrops.push_back(new Object(-1, -1, 1, true, "IT SMELLS A LITTLE WEIRD", "RAW MONSTER MEAT", "monster_meat"));
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

        // triggering map view
        if (e.type == SDL_KEYDOWN && e.button.button == SDL_SCANCODE_M && e.key.repeat == 0) {
            mapView = !mapView;
            if (inventoryView) { inventoryView = false; }
        }

        if (e.type == SDL_KEYDOWN && e.button.button == SDL_SCANCODE_TAB && e.key.repeat == 0) {
            selectedPlayerAction = NONE;
            selectedInventoryItem = nullptr;
            inventoryView = !inventoryView;
            visualsManager->setUITextboxText(" ...");
            if (mapView) { mapView = false; }
        }

        // see if user is trying to switch actions
        if (e.type == SDL_KEYDOWN && keyState[SDL_SCANCODE_2] && e.key.repeat == 0) {
            if (selectedPlayerAction == MOVE) {
                selectedPlayerAction = NONE;
                visualsManager->setUITextboxText(" ...");
            }
            else
            {
                selectedPlayerAction = MOVE;
                visualsManager->setUITextboxText("SELECTED MOVE");
            }
        }
        if (e.type == SDL_KEYDOWN && keyState[SDL_SCANCODE_3] && e.key.repeat == 0) {

            if (selectedPlayerAction == ATTACK) {
                selectedPlayerAction = NONE;
                visualsManager->setUITextboxText(" ...");
            }
            else
            {
                selectedPlayerAction = ATTACK;
                visualsManager->setUITextboxText("SELECTED ATTACK");
            }
        }

        // Input in inventory view
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && inventoryView) {
            // interacting with inventory view
            if(e.button.x > UI_INVENTORY_START_X && e.button.x < UI_INVENTORY_START_X + UI_INVENTORY_WIDTH
                && e.button.y > UI_INVENTORY_START_Y && e.button.y < UI_INVENTORY_START_Y + UI_INVENTORY_HEIGHT)
            {
                int invIdx = getInventoryIndexAtClick(e.button.x, e.button.y);
                try {
                    Object* selectedInventoryObject = &playerInventory.at(invIdx);
                    visualsManager->setUITextboxText(selectedInventoryObject->getDescription());
                    selectedInventoryItem = selectedInventoryObject;
                }
                catch (const std::out_of_range& e) {
                    visualsManager->setUITextboxText("EMPTY INVENTORY SLOT");
                    selectedInventoryItem = nullptr;
                }
            }
            // interacting with inventory UI
            else if (e.button.x > UI_INVENTORY_START_X && e.button.x < UI_INVENTORY_START_X + UI_INVENTORY_WIDTH
                && e.button.y > GAMEVIEW_HEIGHT) {
                
                switch (visualsManager->checkInventoryUIButtonPress(e.button.x, e.button.y, selectedInventoryItem)) {
                    case USE_INVENTORY_ITEM:
                        selectedPlayerAction = USE_INVENTORY_ITEM;
                        processPlayerInventoryUseItem();
                        break;
                    case DROP_INVENTORY_ITEM:
                        selectedPlayerAction = DROP_INVENTORY_ITEM;
                        processPlayerInventoryDropItem();
                        break;
                    default:
                        selectedPlayerAction = NONE;
                        visualsManager->setUITextboxText(" ...");
                        break;
                }

            }
        }

        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && !mapView && !inventoryView) {
            
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
                    processPlayerAttack(mouseX, mouseY);
                    break;
                case END_TURN: // SHOULD NEVER BE AN OPTION
                    std::cout << "Attempting END TURN\n";
                    break;
                case MOVE:
                    processPlayerMove(mouseX, mouseY);
                    break;
                default: // when user clicks on gameview with no selected action
                    if (curRoom->getTile(mouseX, mouseY)->getIsOccupied() || curRoom->getObjectAt(mouseX, mouseY)) {
                        if(curRoom->getNPCAt(mouseX, mouseY)) // found an npc
                        {
                            visualsManager->setFocusedNPC(curRoom->getNPCAt(mouseX, mouseY));
                            visualsManager->setUITextboxText(curRoom->getNPCAt(mouseX, mouseY)->getDescription());
                        }
                        else if (curRoom->getObjectAt(mouseX, mouseY)) // found object
                        {
                            visualsManager->setFocusedNPC(nullptr);
                            visualsManager->setUITextboxText(curRoom->getObjectAt(mouseX, mouseY)->getDescription());
                        }
                    }
                    else {
                        visualsManager->setFocusedNPC(nullptr);
                        visualsManager->setUITextboxText(curRoom->getTile(mouseX, mouseY)->getTileDescription());
                    }
                    break;
                }
            }
        }
    }

    if (mapView || inventoryView) {
        return;
    }

    // Handle continuous key presses with timing
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > MOVE_DELAY) {

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
                if (curRoom && curRoom->isWalkable(deltaX + player->getX(), deltaY + player->getY()) 
                    && !curRoom->getTile(deltaX + player->getX(), deltaY + player->getY())->getIsOccupied()) {
                    player->setPosition(deltaX + player->getX(), deltaY + player->getY());
                    lastMoveTime = currentTime;

                    // Check for room transitions
                    int newPlayerX, newPlayerY;
                    if (dungeon->tryRoomTransition(player->getX(), player->getY(), newPlayerX, newPlayerY)) {
                        player->setPosition(newPlayerX, newPlayerY);
                    }

                    // Check for pick ups
                    if (curRoom->getObjectAt(player->getX(), player->getY())) {
                        if (playerInventory.size() == player->getMaxInventorySize()) {
                            visualsManager->setUITextboxText("NOT ENOUGH ROOM IN INVENTORY.");
                        }
                        else {
                            playerPickUpObject(curRoom->getObjectAt(player->getX(), player->getY()));
                        }
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
        player->setActionCountRemaining(player->getMaxActionCount());
        player->setBonusActionCountRemaining(player->getMaxBonusActionCount());
    }

    if (selectedPlayerAction == END_TURN) {
        player->setMovementSpeedLeft(player->getMovementSpeed());
        player->setActionCountRemaining(player->getMaxActionCount());
        player->setBonusActionCountRemaining(player->getMaxBonusActionCount());
        selectedPlayerAction = NONE;

        processNPCLogic();
    }

}

void Game::playerPickUpObject(Object* obj) {
    if (obj->getName() == "GOLD COINS") {
        int randCoinAmount = getRandomIntInRange(1, 5);
        player->setPlayerGold(player->getPlayerGold() + randCoinAmount);

        visualsManager->setUITextboxText("PICKED UP " + std::to_string(randCoinAmount) + " COIN" + (randCoinAmount > 1 ? "S" : "") + ".");
    }
    else {
        visualsManager->setUITextboxText("PICKED UP " + obj->getName() + ".");
        playerInventory.push_back(*obj);
    }
    dungeon->getCurrentRoom()->removeObject(obj);
}

void Game::processPlayerMove(int mouseX, int mouseY) {

    Tile* moveToTile = dungeon->getCurrentRoom()->getTile(mouseX, mouseY);

    if (findDistanceInTiles(player->getX(), player->getY(), mouseX, mouseY) > player->getMovementSpeedLeft()) {
        visualsManager->setUITextboxText("TOO FAR AWAY TO MOVE THERE.");
    }
    else if (findPathForPlayer(dungeon->getCurrentRoom(), player->getX(), player->getY(), mouseX, mouseY, player->getMovementSpeedLeft()).size() < 1) {
        visualsManager->setUITextboxText("NO PATH TO MOVE THERE.");
    }
    else if (!moveToTile->getIsWalkable() || moveToTile->getIsOccupied() || moveToTile->getType() == DOOR) {
        visualsManager->setUITextboxText("INVALID LOCATION.");
    }
    else {
        player->setMovementSpeedLeft(player->getMovementSpeedLeft() - findDistanceInTiles(mouseX, mouseY, player->getX(), player->getY()));
        player->setPosition(mouseX, mouseY);

        Object* isThereObjectAtTile = dungeon->getCurrentRoom()->getObjectAt(mouseX, mouseY);
        if (isThereObjectAtTile) {
            playerPickUpObject(isThereObjectAtTile);
        }

    }
}

void Game::processPlayerAttack(int mouseX, int mouseY) {

    std::string ActionDisplayString = "";

    int distanceToDesiredPoint = findDistance(mouseX, mouseY, player->getX(), player->getY());
    if (distanceToDesiredPoint > player->getAttackRange()) {
        ActionDisplayString = "TOO FAR TO ATTACK.";
    }
    else if (player->getActionCountRemaining() < 1) {
        ActionDisplayString = "NOT ENOUGH ACTIONS LEFT TO ATTACK.";
    }
    else if (!dungeon->getCurrentRoom()->getTile(mouseX, mouseY)->getIsOccupied() && !dungeon->getCurrentRoom()->getObjectAt(mouseX, mouseY)) {
        ActionDisplayString = "YOU DIDN\'T HIT ANYTHING.";
        player->setActionCountRemaining(player->getActionCountRemaining() - 1);
    }
    else 
    {
        Room* curRoom = dungeon->getCurrentRoom();
        if (curRoom->getNPCAt(mouseX, mouseY)) { // hit npc
            NPC* npc = curRoom->getNPCAt(mouseX, mouseY);

            if (ActionDisplayString.size() > 0) {
                ActionDisplayString += "\n";
            }

            int remainingHealth = npc->getHealthPoints() - player->getDamage();
            ActionDisplayString += "HIT " + npc->getName() + " FOR " + std::to_string(player->getDamage()) + " DAMAGE.";
            if (remainingHealth > 0) {
                npc->setHealthPoints(remainingHealth);
            }
            else {

                if (ActionDisplayString.size() > 0) {
                    ActionDisplayString += "\n";
                }

                std::vector<NPC*>* npcs = curRoom->getListOfNPCs();

                // RNG item drop
                genRandomDrop(npc->getX(), npc->getY());

                visualsManager->setFocusedNPC(nullptr);
                ActionDisplayString += npc->getName() + " DIED.";
                delete npc;
                npcs->erase(std::remove(npcs->begin(), npcs->end(), npc), npcs->end());

                dungeon->getCurrentRoom()->getTile(mouseX, mouseY)->setIsOccupied(false);
            }

        } 
        else if (curRoom->getObjectAt(mouseX, mouseY)) { // hit object
            Object* obj = curRoom->getObjectAt(mouseX, mouseY);

            if (ActionDisplayString.size() > 0) {
                ActionDisplayString += "\n";
            }

            int remainingHitPoints = obj->getHitPoints() - player->getDamage();
            ActionDisplayString += "HIT " + obj->getName() + " FOR " + std::to_string(player->getDamage()) + " DAMAGE.";
            if (remainingHitPoints > 0) {
                obj->setHitPoints(remainingHitPoints);
            }
            else {

                if (ActionDisplayString.size() > 0) {
                    ActionDisplayString += "\n";
                }

                std::vector<Object*>* objects = curRoom->getObjects();

                ActionDisplayString += obj->getName() + " BROKE INTO PIECES.";
                genRandomDrop(mouseX, mouseY);
                delete obj;
                objects->erase(std::remove(objects->begin(), objects->end(), obj), objects->end());

                dungeon->getCurrentRoom()->getTile(mouseX, mouseY)->setIsOccupied(false);
            }
        }

        player->setActionCountRemaining(player->getActionCountRemaining() - 1);

    }

    visualsManager->setUITextboxText(ActionDisplayString);
}

void Game::processPlayerInventoryUseItem() {
    if (!selectedInventoryItem) {
        return;
    }

    std::string itemName = selectedInventoryItem->getName();
    std::string result = "";
    if (itemName == "HEALTH POTION") {
        
        player->setHealthPoints(std::min(player->getMaxHealthPoints(), player->getHealthPoints() + 5));

        result += "DRANK " + selectedInventoryItem->getName() + ".\n";
        result += "RECOVERED 5 HIT POINTS.";
        visualsManager->setUITextboxText(result);

    }
    else if (itemName == "ENERGY POTION") {

        player->setMovementSpeedLeft(std::min(player->getMovementSpeed(), player->getMovementSpeedLeft() + 3));

        result += "DRANK " + selectedInventoryItem->getName() + ".\n";
        result += "RECOVERED 3 MOVEMENT POINTS.";
        visualsManager->setUITextboxText(result);
    }
    else if (itemName == "STIR FRY") {

        player->setHealthPoints(std::min(player->getMaxHealthPoints(), player->getHealthPoints() + 3));

        result += "ATE " + selectedInventoryItem->getName() + ".\n";
        result += "RECOVERED 3 HIT POINTS.";
        visualsManager->setUITextboxText(result);
    }
    else if (itemName == "RAW MONSTER MEAT") {

        player->setHealthPoints(std::min(player->getMaxHealthPoints(), player->getHealthPoints() + 3));

        result += "ATE " + selectedInventoryItem->getName() + ".\n";
        result += "RECOVERED 3 HIT POINTS.";
        visualsManager->setUITextboxText(result);
    }
    removePlayerInventoryItem(selectedInventoryItem);
    selectedInventoryItem = nullptr;
}

void Game::processPlayerInventoryDropItem() {
    Room* curRoom = dungeon->getCurrentRoom();
    int pX = player->getX();
    int pY = player->getY();

    int dropX = -1;
    int dropY = -1;

    // finding nearest empty spot
    if (pX - 1 >= 0 && curRoom->getTile(pX - 1, pY)->getIsWalkable() && !curRoom->getTile(pX - 1, pY)->getIsOccupied() && !curRoom->getObjectAt(pX - 1, pY)) {
        dropX = pX - 1;
        dropY = pY;
    }
    else if (pY - 1 >= 0 && curRoom->getTile(pX, pY - 1)->getIsWalkable() && !curRoom->getTile(pX, pY - 1)->getIsOccupied() && !curRoom->getObjectAt(pX, pY - 1)) {
        dropX = pX;
        dropY = pY - 1;
    }
    else if (pX + 1 < ROOM_WIDTH && curRoom->getTile(pX + 1, pY)->getIsWalkable() && !curRoom->getTile(pX + 1, pY)->getIsOccupied() && !curRoom->getObjectAt(pX + 1, pY)) {
        dropX = pX + 1;
        dropY = pY;
    } 
    else if (pY + 1 < ROOM_HEIGHT && curRoom->getTile(pX, pY + 1)->getIsWalkable() && !curRoom->getTile(pX, pY + 1)->getIsOccupied() && !curRoom->getObjectAt(pX, pY + 1)) {
        dropX = pX;
        dropY = pY + 1;
    }

    if (dropX < 0 || dropY < 0) {
        visualsManager->setUITextboxText("NO ROOM TO DROP ITEM.");
    }
    else {
        Object* droppedObj = new Object(
            dropX, 
            dropY, 
            selectedInventoryItem->getHitPoints(),
            selectedInventoryItem->getIsCollectable(),
            selectedInventoryItem->getDescription(),
            selectedInventoryItem->getName(),
            selectedInventoryItem->getTextureID());

        curRoom->addObjectToRoom(droppedObj);

        visualsManager->setUITextboxText("DROPPED " + selectedInventoryItem->getName() + ".");

        removePlayerInventoryItem(selectedInventoryItem);
        selectedInventoryItem = nullptr;
    }
}

void Game::removePlayerInventoryItem(Object* obj) {
    for (auto it = playerInventory.begin(); it != playerInventory.end(); ++it) {
        if (&(*it) == obj) {
            playerInventory.erase(it);
            break;
        }
    }
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
        std::string name = npc->getName();

        if (NPCActionDisplayString.size() > 0) {
            NPCActionDisplayString += "\n";
        }

        switch (actionTaken) {
        case NPC_ATTACK_PLAYER:
            NPCActionDisplayString += name + " ATTACKED PLAYER FOR " + std::to_string(npc->getDamage()) + " DAMAGE";
            player->setHealthPoints(player->getHealthPoints() - npc->getDamage());
            break;
        case NPC_MOVE:
            NPCActionDisplayString += name + " MOVED";

            currentRoom->getTile(oldNpcX, oldNpcY)->setIsOccupied(false);
            currentRoom->getTile(npc->getX(), npc->getY())->setIsOccupied(true);

            break;
        default:
            NPCActionDisplayString += "NO ACTION TAKEN BY " + name;
            break;
        }
    }
    visualsManager->setUITextboxText(NPCActionDisplayString);
}

void Game::genRandomDrop(int x, int y) {

    Room* curRoom = dungeon->getCurrentRoom();

    if (!getRandomIntInRange(0, 2)) { // 33% chance to drop something

        Object* refObj = RNGObjectDrops.at(getRandomIntInRange(0, RNGObjectDrops.size()-1));

        Object* obj = new Object(x, y, 
            refObj->getHitPoints(), 
            refObj->getIsCollectable(), 
            refObj->getDescription(),
            refObj->getName(),
            refObj->getTextureID());
        curRoom->addObjectToRoom(obj);
    }
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
            visualsManager->render(currentRoom, player.get(), &playerInventory, selectedPlayerAction, inventoryView, selectedInventoryItem);
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