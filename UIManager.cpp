#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* SDLRenderer) : renderer(SDLRenderer), frameCount(0) {
	loadTextures();
    loadNPCTextures();
    loadUIButtons();

    gameView = { UI_SIDE_PANEL_WIDTH, 0, GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT };
    leftUIPanel = { 0, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    rightUIPanel = { UI_SIDE_PANEL_WIDTH + GAMEVIEW_WIDTH, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    bottomUIPanel = { UI_BOTTOM_PANNEL_START_X, UI_BOTTOM_PANNEL_START_Y, UI_BOTTOM_PANNEL_WIDTH, UI_BOTTOM_PANNEL_HEIGHT };

}

UIManager::~UIManager() {
    if (wallTexture) {
        SDL_DestroyTexture(wallTexture);
    }
    if (doorTexture) {
        SDL_DestroyTexture(doorTexture);
    }
    if (NPCTexture) {
        SDL_DestroyTexture(NPCTexture);
    }
}

void UIManager::loadTexture(const char* filePath, SDL_Texture*& destinationTexture) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    destinationTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!destinationTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

void UIManager::loadTextures() {

    // loading cursor
    SDL_Surface* surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/cursor_2.png");
    if (surface) {
        cursorTexture = SDL_CreateColorCursor(surface, surface->w / 2, surface->h / 2);
        SDL_FreeSurface(surface);

        if (!cursorTexture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        }
        else {
            SDL_SetCursor(cursorTexture);
        }
    }
    else {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight.png", playerTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/wall.png", wallTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/wood_door.png", doorTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/drop_ladder.png", ladderTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/dirt_1.png", floorTexture);

}

void UIManager::loadNPCTextures() {
    SDL_Texture* goblinTexture = nullptr;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/goblin.png", goblinTexture);
    NPCTextures[GOBLIN] = goblinTexture;

    SDL_Texture* spiderTexture = nullptr;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/spider.png", spiderTexture);
    NPCTextures[SPIDER] = spiderTexture;
}

void UIManager::loadUIButtons() {
    
    SDL_Texture* buttonActive = nullptr;
    SDL_Texture* buttonInactive = nullptr;

    SDL_Rect attackButtonFrame = { GAMEVIEW_START_X + (2 * (BUTTON_WIDTH + (TILE_SIZE / 2))), UI_BOTTOM_PANNEL_START_Y + (TILE_SIZE / 2), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/sword_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/sword_bnw_clicked.png", buttonActive);

    attackButton = new UIButton(attackButtonFrame, buttonActive, buttonInactive, INACTIVE, ATTACK);

    buttonActive = nullptr;
    buttonInactive = nullptr;

    SDL_Rect moveButtonFrame = { GAMEVIEW_START_X + BUTTON_WIDTH + (TILE_SIZE / 2), UI_BOTTOM_PANNEL_START_Y + (TILE_SIZE / 2), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/move_button_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/move_button_bnw_clicked.png", buttonActive);

    moveButton = new UIButton(moveButtonFrame, buttonActive, buttonInactive, INACTIVE, MOVE);

    buttonActive = nullptr;
    buttonInactive = nullptr;

    SDL_Rect endTurnButtonFrame = { GAMEVIEW_START_X, UI_BOTTOM_PANNEL_START_Y + (TILE_SIZE / 2), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/x_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/x_bnw_clicked.png", buttonActive);

    endTurnButton = new UIButton(endTurnButtonFrame, buttonActive, buttonInactive, INACTIVE, END_TURN);
}

void UIManager::renderGameView(Room* currentRoom, Player* player) {
    renderCurrentRoom(currentRoom);
    renderPlayer(player->getXTile(), player->getYTile(), player->getWhichDirectionIsFacing());
    renderCurrentRoomObjects(currentRoom);
    renderCurrentRoomNPCs(currentRoom);
    renderDarkness(player->getXTile(), player->getYTile());

    //renderActionableTiles(player->getXTile(), player->getYTile());
};

void UIManager::renderUIPanel(SDL_Rect panel) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &panel);
};

void UIManager::renderUIButton(UIButton* button) {

    switch (button->getButtonState()) {
    case ACTIVE:
        SDL_RenderCopy(renderer, button->getButtonActiveTexture(), nullptr, &button->getButtonArea());
        break;
    default:
        SDL_RenderCopy(renderer, button->getButtonInactiveTexture(), nullptr, &button->getButtonArea());
        break;
    }

    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &button->getButtonArea());
};

void UIManager::renderUI() {
    renderUIPanel(leftUIPanel);
    renderUIPanel(rightUIPanel);
    renderUIPanel(bottomUIPanel);

    renderUIButton(attackButton);
    renderUIButton(moveButton);
    renderUIButton(endTurnButton);
};

void UIManager::renderPlayer(int playerLocationX, int playerLocationY, int facingDirection) {
    SDL_Rect destRect = {
        playerLocationX,
        playerLocationY,
        TILE_SIZE,
        TILE_SIZE
    };
    
    SDL_RenderCopyEx(renderer, playerTexture, nullptr, &destRect, 0.0, nullptr, facingDirection ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

}

void UIManager::renderCurrentRoom(Room* currentRoom) {
    for (int y = 0; y < currentRoom->getHeight(); y++) {
        for (int x = 0; x < currentRoom->getWidth(); x++) {
            SDL_Rect tileRect = { (x * TILE_SIZE) + GAMEVIEW_START_X,
                                    (y * TILE_SIZE) + GAMEVIEW_START_Y,
                                    TILE_SIZE,
                                    TILE_SIZE };

            switch (currentRoom->getTile(x, y)) {
            case WALL:
                SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect);
                break;
            case DOOR:
                SDL_RenderCopy(renderer, doorTexture, nullptr, &tileRect);
                break;
            case FLOOR:
                SDL_RenderCopy(renderer, floorTexture, nullptr, &tileRect);
                break;
            case LADDER:
                SDL_RenderCopy(renderer, ladderTexture, nullptr, &tileRect);
                break;
            }
        }
    }
};

void UIManager::renderCurrentRoomNPCs(Room* currentRoom) {

    if (currentRoom->getListOfNPCs()->size() > 0) {
        std::vector<NPC*>* npcs = currentRoom->getListOfNPCs();
        for (NPC* npc : *npcs) {
            int location_x = npc->getX();
            int location_y = npc->getY();
            int typeID = npc->getTypeID();

            SDL_Rect NPCFrame = { (location_x * TILE_SIZE) + GAMEVIEW_START_X,
                                  (location_y * TILE_SIZE) + GAMEVIEW_START_Y,
                                   TILE_SIZE,
                                   TILE_SIZE };

            try {
                SDL_RenderCopy(renderer, NPCTextures[typeID], nullptr, &NPCFrame);
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Caught exception during NPC texture rendering: " << e.what() << std::endl;
            }

        }
    }
};

void UIManager::renderCurrentRoomObjects(Room* currentRoom) {

}

void UIManager::renderDarkness(int playerLocationX, int playerLocationY) {
    int gameViewWidthInTiles = GAMEVIEW_WIDTH / TILE_SIZE;
    int gameViewHeightInTiles = GAMEVIEW_HEIGHT / TILE_SIZE;

    float time = SDL_GetTicks() / 1000.0f;  // Get time in seconds
    float brightnessOscillation = 1.0f + 0.05f * std::sin(time * 3.0f);

    for (int i = 0; i < gameViewWidthInTiles; i++) {
        for (int j = 0; j < gameViewHeightInTiles; j++) {

            int darknessFactor = findTileDistanceForShading(playerLocationX, playerLocationY, GAMEVIEW_START_X + (i * TILE_SIZE), GAMEVIEW_START_Y + (j * TILE_SIZE));
            if (darknessFactor > MAX_DARKNESS_RANGE_IN_TILES) {
                darknessFactor = (MAX_DARKNESS_RANGE_IN_TILES + 1) * DARKNESS_SCALE;
            }
            else {
                darknessFactor *= DARKNESS_SCALE;
                darknessFactor = static_cast<int>(darknessFactor * brightnessOscillation);

            }


            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, darknessFactor);
            SDL_Rect darknessOverlay = { GAMEVIEW_START_X + (i * TILE_SIZE), GAMEVIEW_START_Y + (j * TILE_SIZE), TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &darknessOverlay);
        }
    }
}

void UIManager::render(Room* currentRoom, Player* player, int selectedPlayerAction) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    renderUI();
    updateUIButtonsBasedOnSelectedAction(selectedPlayerAction);
    renderGameView(currentRoom, player);

    if (frameCount > MAX_FRAME_COUNT) {
        frameCount = 0;
    }
    frameCount++;
};

int UIManager::checkUIButtonPress(int mouseX, int mouseY) {

    if ((mouseX > attackButton->getButtonArea().x && mouseX < attackButton->getButtonArea().x + BUTTON_WIDTH)
        && (mouseY > attackButton->getButtonArea().y && mouseY < attackButton->getButtonArea().y + BUTTON_HEIGHT)) {
        
        moveButton->setButtonState(INACTIVE);
        endTurnButton->setButtonState(INACTIVE);
        
        attackButton->setButtonState(ACTIVE);
        return ATTACK;
    }

    if ((mouseX > moveButton->getButtonArea().x && mouseX < moveButton->getButtonArea().x + BUTTON_WIDTH)
        && (mouseY > moveButton->getButtonArea().y && mouseY < moveButton->getButtonArea().y + BUTTON_HEIGHT)) {

        endTurnButton->setButtonState(INACTIVE);
        attackButton->setButtonState(INACTIVE);

        moveButton->setButtonState(ACTIVE);
        return MOVE;
    }

    if ((mouseX > endTurnButton->getButtonArea().x && mouseX < endTurnButton->getButtonArea().x + BUTTON_WIDTH)
        && (mouseY > endTurnButton->getButtonArea().y && mouseY < endTurnButton->getButtonArea().y + BUTTON_HEIGHT)) {

        moveButton->setButtonState(INACTIVE);
        attackButton->setButtonState(INACTIVE);

        endTurnButton->setButtonState(ACTIVE);
        return END_TURN;
    }

    return NONE;
} 

void UIManager::updateUIButtonsBasedOnSelectedAction(int selectedPlayerAction) {
    switch (selectedPlayerAction) {
    case ATTACK:
        moveButton->setButtonState(INACTIVE);
        endTurnButton->setButtonState(INACTIVE);
        attackButton->setButtonState(ACTIVE);
        break;
    case END_TURN:
        moveButton->setButtonState(INACTIVE);
        endTurnButton->setButtonState(ACTIVE);
        attackButton->setButtonState(INACTIVE);
        break;
    case MOVE:
        moveButton->setButtonState(ACTIVE);
        endTurnButton->setButtonState(INACTIVE);
        attackButton->setButtonState(INACTIVE);
        break;
    default:
        moveButton->setButtonState(INACTIVE);
        endTurnButton->setButtonState(INACTIVE);
        attackButton->setButtonState(INACTIVE);
        break;
    }
}

void UIManager::renderActionableTiles(int playerX, int playerY) {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_Rect n = {playerX - (TILE_SIZE), playerY, TILE_SIZE, TILE_SIZE};
    SDL_RenderDrawRect(renderer, &n);
}