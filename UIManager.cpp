#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* SDLRenderer) : renderer(SDLRenderer), frameCount(0), currentTextBoxText("") {
	loadTextures();
    loadAlphabetTextures();
    loadNPCTextures();
    loadUIButtons();

    gameView = { UI_SIDE_PANEL_WIDTH, 0, GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT };
    leftUIPanel = { 0, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    rightUIPanel = { UI_SIDE_PANEL_WIDTH + GAMEVIEW_WIDTH, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    bottomUIPanel = { UI_BOTTOM_PANNEL_START_X, UI_BOTTOM_PANNEL_START_Y, UI_BOTTOM_PANNEL_WIDTH, UI_BOTTOM_PANNEL_HEIGHT };

    UITextBoxFrame = { UI_TEXTBOX_START_X, UI_TEXTBOX_START_Y, UI_TEXTBOX_WIDTH, UI_TEXTBOX_HEIGHT };

    UIMiniMapFrame = { UI_MINIMAP_START_X, UI_MINIMAP_START_Y, UI_MINIMAP_WIDTH, UI_MINIMAP_HEIGHT };

    std::cout << "Successfully intialized UIManager.\n===============================\n";
}

UIManager::~UIManager() {
    // TODO: implement so it clears the map of textures

    /*if (NPCTexture) {
        SDL_DestroyTexture(NPCTexture);
    }*/
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

void UIManager::loadTileTexture(const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    SDL_Texture* newTileTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!newTileTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
    std::string newTileTextureID = extractFileName(filePath);

    TileTextures[newTileTextureID] = newTileTexture;
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

    loadPlayerTextures();

    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/stone_wall.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/wood_door.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/drop_ladder.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/dirt_1.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/dirt_2.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/dirt_3.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/tree_1.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/tree_2.png");

    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/chest.png");

    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/full_red_heart.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/half_red_heart.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/empty_heart.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/movement_point.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/empty_movement_point.png");

    gameOverTextureFrame = { (SCREEN_WIDTH / 2) - (TILE_SIZE * 4),(SCREEN_HEIGHT / 2) - (TILE_SIZE * 2), TILE_SIZE * 8, TILE_SIZE * 4};
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_1.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_2.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_3.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_4.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_5.png");
    loadTileTexture("C:/Users/theda/source/repos/Monolith_DND/game_over_6.png");

    std::cout << "Successfully loaded base textures.\n";

}

void UIManager::loadPlayerTextures() {
    SDL_Texture* playerTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight_1.png", playerTexture);
    playerTextures.push_back(playerTexture);

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight_2.png", playerTexture);
    playerTextures.push_back(playerTexture);

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight_3.png", playerTexture);
    playerTextures.push_back(playerTexture);

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight_4.png", playerTexture);
    playerTextures.push_back(playerTexture);
}

void UIManager::loadAlphabetTextures() {
    SDL_Texture* letterTexture;

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/prompt_start.png", letterTexture); AlphabetTextures['>'] = letterTexture;

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_a.png", letterTexture);AlphabetTextures['A'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_b.png", letterTexture);AlphabetTextures['B'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_c.png", letterTexture);AlphabetTextures['C'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_d.png", letterTexture);AlphabetTextures['D'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_e.png", letterTexture);AlphabetTextures['E'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_f.png", letterTexture);AlphabetTextures['F'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_g.png", letterTexture);AlphabetTextures['G'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_h.png", letterTexture);AlphabetTextures['H'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_i.png", letterTexture);AlphabetTextures['I'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_j.png", letterTexture);AlphabetTextures['J'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_k.png", letterTexture);AlphabetTextures['K'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_l.png", letterTexture);AlphabetTextures['L'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_m.png", letterTexture);AlphabetTextures['M'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_n.png", letterTexture);AlphabetTextures['N'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_o.png", letterTexture);AlphabetTextures['O'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_p.png", letterTexture);AlphabetTextures['P'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_q.png", letterTexture);AlphabetTextures['Q'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_r.png", letterTexture);AlphabetTextures['R'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_s.png", letterTexture);AlphabetTextures['S'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_t.png", letterTexture);AlphabetTextures['T'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_u.png", letterTexture);AlphabetTextures['U'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_v.png", letterTexture);AlphabetTextures['V'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_w.png", letterTexture);AlphabetTextures['W'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_x.png", letterTexture);AlphabetTextures['X'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_y.png", letterTexture);AlphabetTextures['Y'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/upper_z.png", letterTexture);AlphabetTextures['Z'] = letterTexture;

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_1.png", letterTexture); AlphabetTextures['1'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_2.png", letterTexture); AlphabetTextures['2'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_3.png", letterTexture); AlphabetTextures['3'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_4.png", letterTexture); AlphabetTextures['4'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_5.png", letterTexture); AlphabetTextures['5'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_6.png", letterTexture); AlphabetTextures['6'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_7.png", letterTexture); AlphabetTextures['7'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_8.png", letterTexture); AlphabetTextures['8'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_9.png", letterTexture); AlphabetTextures['9'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/num_0.png", letterTexture); AlphabetTextures['0'] = letterTexture;

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/..png", letterTexture); AlphabetTextures['.'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/colon.png", letterTexture); AlphabetTextures[':'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/semi_colon.png", letterTexture); AlphabetTextures[';'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/!.png", letterTexture); AlphabetTextures['!'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/question_mark.png", letterTexture); AlphabetTextures['?'] = letterTexture;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/'.png", letterTexture); AlphabetTextures['\''] = letterTexture;

    std::cout << "Successfully loaded alphabet textures.\n";

}

void UIManager::loadNPCTextures() {
    SDL_Texture* goblinTexture = nullptr;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/goblin.png", goblinTexture);
    NPCTextures[GOBLIN] = goblinTexture;

    SDL_Texture* spiderTexture = nullptr;
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/spider.png", spiderTexture);
    NPCTextures[SPIDER] = spiderTexture;

    std::cout << "Successfully loaded NPC textures.\n";

}

void UIManager::loadUIButtons() {
    
    SDL_Texture* buttonActive = nullptr;
    SDL_Texture* buttonInactive = nullptr;

    SDL_Rect attackButtonFrame = { GAMEVIEW_START_X + (2 * (BUTTON_WIDTH + (TILE_SIZE / 2))), UI_BOTTOM_PANNEL_START_Y + (0.5 * TILE_SIZE), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/sword_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/sword_bnw_clicked.png", buttonActive);

    attackButton = new UIButton(attackButtonFrame, buttonActive, buttonInactive, INACTIVE, ATTACK);

    buttonActive = nullptr;
    buttonInactive = nullptr;

    SDL_Rect moveButtonFrame = { GAMEVIEW_START_X + BUTTON_WIDTH + (TILE_SIZE / 2), UI_BOTTOM_PANNEL_START_Y + (0.5 * TILE_SIZE), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/move_button_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/move_button_bnw_clicked.png", buttonActive);

    moveButton = new UIButton(moveButtonFrame, buttonActive, buttonInactive, INACTIVE, MOVE);

    buttonActive = nullptr;
    buttonInactive = nullptr;

    SDL_Rect endTurnButtonFrame = { GAMEVIEW_START_X, UI_BOTTOM_PANNEL_START_Y + (0.5 * TILE_SIZE), BUTTON_WIDTH, BUTTON_HEIGHT };
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/x_bnw_non_clicked.png", buttonInactive);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/x_bnw_clicked.png", buttonActive);

    endTurnButton = new UIButton(endTurnButtonFrame, buttonActive, buttonInactive, INACTIVE, END_TURN);

    std::cout << "Successfully loaded button textures.\n";

}

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

void UIManager::renderUITextBox() {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &UITextBoxFrame);

    if(currentTextBoxText.size() > 0)
    {
        int startRenderX = UI_TEXTBOX_START_X;
        int startRenderY = UI_TEXTBOX_START_Y;
        int charOffset = 0;

        SDL_Rect charFrame = { startRenderX + (UI_TEXTBOX_CHAR_SIZE * charOffset) / 2,
                            startRenderY,
                            UI_TEXTBOX_CHAR_SIZE, UI_TEXTBOX_CHAR_SIZE };
        SDL_RenderCopy(renderer, AlphabetTextures['>'], nullptr, &charFrame);

        charOffset++;

        for (char c : currentTextBoxText) {
            if (c == '\n') { // rendering new line of text

                charOffset = 0;

                startRenderY += UI_TEXTBOX_CHAR_SIZE;
                startRenderX = UI_TEXTBOX_START_X;
                charFrame = { startRenderX + (UI_TEXTBOX_CHAR_SIZE * charOffset) / 2,
                            startRenderY,
                            UI_TEXTBOX_CHAR_SIZE, UI_TEXTBOX_CHAR_SIZE };
                SDL_RenderCopy(renderer, AlphabetTextures['>'], nullptr, &charFrame);

                charOffset++;
            }
            else {

                charFrame = { startRenderX + (UI_TEXTBOX_CHAR_SIZE * charOffset) / 2,
                            startRenderY,
                            UI_TEXTBOX_CHAR_SIZE, UI_TEXTBOX_CHAR_SIZE };

                SDL_RenderCopy(renderer, AlphabetTextures[c], nullptr, &charFrame);

                charOffset++;
            }
        }
    }
}

void UIManager::setUITextboxText(std::string text) {
    currentTextBoxText = text;
}

void UIManager::renderUI(Player* player) {
    renderUIPanel(leftUIPanel);
    renderUIPanel(rightUIPanel);
    renderUIPanel(bottomUIPanel);

    renderUIButton(attackButton);
    renderUIButton(moveButton);
    renderUIButton(endTurnButton);

    renderUITextBox();

    renderPlayerStats(player);

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

void UIManager::renderGameView(Room* currentRoom, Player* player) {
    renderCurrentRoom(currentRoom);
    renderPlayer(player->getXTile(), player->getYTile(), player->getWhichDirectionIsFacing());
    renderCurrentRoomObjects(currentRoom);
    renderCurrentRoomNPCs(currentRoom);
    //renderDarkness(player->getXTile(), player->getYTile());
};

void UIManager::renderPlayer(int playerLocationX, int playerLocationY, int facingDirection) {
    SDL_Rect destRect = {
        playerLocationX,
        playerLocationY,
        TILE_SIZE,
        TILE_SIZE
    };
    int curPlayerFrame = frameCount / (MAX_FRAME_COUNT / playerTextures.size());
    SDL_RenderCopyEx(renderer, playerTextures[curPlayerFrame], nullptr, &destRect, 0.0, nullptr, facingDirection ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

}

void UIManager::renderPlayerStats(Player* player) {

    // health
    int healthRenderStartX = TILE_SIZE;
    int healthRenderStartY = TILE_SIZE;

    int healthPoints = player->getHealthPoints();
    int emptyHealth = player->getMaxHealthPoints() - healthPoints;

    int curHeartOffset = 0;
    while (healthPoints > 1) {
        int renderStartX = healthRenderStartX + (curHeartOffset * TILE_SIZE / 1.25);
        SDL_Rect heartArea = { renderStartX , healthRenderStartY, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, TileTextures["full_red_heart"], nullptr, &heartArea);
        curHeartOffset++;
        healthPoints -= 2;
    }
    if (healthPoints == 1) {
        int renderStartX = healthRenderStartX + (curHeartOffset * TILE_SIZE / 1.25);
        SDL_Rect heartArea = { renderStartX , healthRenderStartY, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, TileTextures["half_red_heart"], nullptr, &heartArea);
        curHeartOffset++;
        emptyHealth--;
    }
    while (emptyHealth > 0) {
        int renderStartX = healthRenderStartX + (curHeartOffset * TILE_SIZE / 1.25);
        SDL_Rect heartArea = { renderStartX , healthRenderStartY, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, TileTextures["empty_heart"], nullptr, &heartArea);
        curHeartOffset++;
        emptyHealth -= 2;
    }

    int movementSpeedLeft = player->getMovementSpeedLeft();
    int emptyMovementSpeed = player->getMovementSpeed() - movementSpeedLeft;

    // movement speed
    int movementSpeedRenderStartX = TILE_SIZE;
    int movementSpeedRenderStartY = 2 * TILE_SIZE;
    int curMPOffset = 0;
    while (movementSpeedLeft > 0) {
        int renderStartX = movementSpeedRenderStartX + (curMPOffset * TILE_SIZE / 2);
        SDL_Rect MPArea = { renderStartX , movementSpeedRenderStartY, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, TileTextures["movement_point"], nullptr, &MPArea);

        curMPOffset++;
        movementSpeedLeft--;
    }
    while (emptyMovementSpeed > 0) {
        int renderStartX = movementSpeedRenderStartX + (curMPOffset * TILE_SIZE / 2);
        SDL_Rect MPArea = { renderStartX , movementSpeedRenderStartY, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, TileTextures["empty_movement_point"], nullptr, &MPArea);

        curMPOffset++;
        emptyMovementSpeed--;
    }
}

void UIManager::renderCurrentRoom(Room* currentRoom) {
    for (int y = 0; y < currentRoom->getHeight(); y++) {
        for (int x = 0; x < currentRoom->getWidth(); x++) {
            SDL_Rect tileRect = { (x * TILE_SIZE) + GAMEVIEW_START_X,
                                    (y * TILE_SIZE) + GAMEVIEW_START_Y,
                                    TILE_SIZE,
                                    TILE_SIZE };
            SDL_RenderCopy(renderer, TileTextures[currentRoom->getTile(x,y)->getTexture()], nullptr, &tileRect);
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

    renderUI(player);
    updateUIButtonsBasedOnSelectedAction(selectedPlayerAction);
    renderGameView(currentRoom, player);

    if (frameCount >= MAX_FRAME_COUNT - 1) {
        frameCount = 0;
    }
    frameCount++;
};

void UIManager::renderDeathScreen() {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    int curFrame = frameCount / (MAX_FRAME_COUNT / 7) + 1;
    if (curFrame > 6) {
        curFrame = 1;
    }
    SDL_RenderCopy(renderer, TileTextures["game_over_" + std::to_string(curFrame)], nullptr, &gameOverTextureFrame);

    if (frameCount >= MAX_FRAME_COUNT - 1) {
        frameCount = 0;
    }
    frameCount++;
}

void UIManager::renderMap(std::map<RoomCoord, std::unique_ptr<Room>>* rooms, RoomCoord curRoomCoord, bool mapMode) {

    int startRenderX = (SCREEN_WIDTH / 2) - (TILE_SIZE / 2);
    int startRenderY = (SCREEN_HEIGHT / 2) - (TILE_SIZE / 2);
    int roomOffsetX = 0;
    int roomOffsetY = 0;
    int tileScale = 1;

    if (!mapMode) { // if we're in mini map mode
        tileScale = 2;
        startRenderX = UI_MINIMAP_START_X + (UI_MINIMAP_WIDTH / 2) - (TILE_SIZE / 4);
        startRenderY = UI_MINIMAP_START_Y + (UI_MINIMAP_HEIGHT / 2) - (TILE_SIZE / 4);
    }
    else {
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
        SDL_RenderClear(renderer);
    }

    for (const auto& [coord, room] : *rooms) {
        if(!mapMode) // setting offset for when we're in minimap mode
        {
            roomOffsetX = coord.x - curRoomCoord.x;
            roomOffsetY = coord.y - curRoomCoord.y;
        }
        else {
            roomOffsetX = coord.x;
            roomOffsetY = coord.y;
        }
        SDL_Rect roomTileFrame = { 
            startRenderX + (roomOffsetX * TILE_SIZE / tileScale),
            startRenderY + (roomOffsetY * TILE_SIZE / tileScale),
            TILE_SIZE / tileScale, TILE_SIZE / tileScale };

        if (curRoomCoord.x == coord.x && curRoomCoord.y == coord.y) {
            room->setRoomNoticedOnMap(true);
            SDL_SetRenderDrawColor(renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, COLOR_BLUE.a);
            SDL_RenderFillRect(renderer, &roomTileFrame);
        } 
        else if(room->hasRoomBeenVisited()) // an already visited room
        {
            room->setRoomNoticedOnMap(true);
            if (room->isRoomEncounter())
            {
                SDL_SetRenderDrawColor(renderer, COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, COLOR_RED.a);
                SDL_RenderFillRect(renderer, &roomTileFrame);
            }
            else {
                SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
                SDL_RenderDrawRect(renderer, &roomTileFrame);
            }
        }
        else if (!room->hasRoomBeenVisited()) { // checking for neighbor rooms

            bool onlyHorizontal = abs(curRoomCoord.x - coord.x) == 1 && abs(curRoomCoord.y - coord.y) == 0;
            bool onlyVertical = abs(curRoomCoord.x - coord.x) == 0 && abs(curRoomCoord.y - coord.y) == 1;

            if (onlyHorizontal || onlyVertical) {
                room->setRoomNoticedOnMap(true);
                SDL_SetRenderDrawColor(renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
                SDL_RenderFillRect(renderer, &roomTileFrame);
            }
            if (room->getRoomNoticedOnMap()) {
                SDL_SetRenderDrawColor(renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
                SDL_RenderFillRect(renderer, &roomTileFrame);
            }

        }
    }

    if (!mapMode) {

        SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
        SDL_RenderDrawRect(renderer, &UIMiniMapFrame);

        // mini map mask
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
        SDL_Rect miniMapMask = {
            UI_MINIMAP_START_X - (TILE_SIZE / 2) + 1,
            UI_MINIMAP_START_Y - (TILE_SIZE / 2) + 1,
            UI_MINIMAP_WIDTH + TILE_SIZE - 2,
            (TILE_SIZE / 2) - 1
        };
        SDL_RenderFillRect(renderer, &miniMapMask);
        miniMapMask = {
            UI_MINIMAP_START_X - (TILE_SIZE / 2) + 1,
            UI_MINIMAP_START_Y - (TILE_SIZE / 2) + 1,
            (TILE_SIZE / 2) - 1,
            UI_MINIMAP_HEIGHT + TILE_SIZE - 3
        };
        SDL_RenderFillRect(renderer, &miniMapMask);
        miniMapMask = {
            UI_MINIMAP_START_X - (TILE_SIZE / 2) + 1,
            UI_MINIMAP_START_Y + UI_MINIMAP_HEIGHT,
            UI_MINIMAP_WIDTH + TILE_SIZE - 2,
            (TILE_SIZE / 2) - 1
        };
        SDL_RenderFillRect(renderer, &miniMapMask);
        miniMapMask = {
            UI_MINIMAP_START_X + UI_MINIMAP_WIDTH,
            UI_MINIMAP_START_Y - (TILE_SIZE / 2) + 1,
            (TILE_SIZE / 2),
            UI_MINIMAP_HEIGHT + TILE_SIZE - 3
        };
        SDL_RenderFillRect(renderer, &miniMapMask);

    }


}


// ANIMATION
