#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* SDLRenderer) : renderer(SDLRenderer) {
	loadTextures();

    gameView = { UI_SIDE_PANEL_WIDTH, 0, GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT };
    leftUIPanel = { 0, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    rightUIPanel = { UI_SIDE_PANEL_WIDTH + GAMEVIEW_WIDTH, 0, UI_SIDE_PANEL_WIDTH, UI_SIDE_PANEL_HEIGHT };
    bottomUIPanel = { UI_BOTTOM_PANNEL_START_X, UI_BOTTOM_PANNEL_START_Y, UI_BOTTOM_PANNEL_WIDTH, UI_BOTTOM_PANNEL_HEIGHT };

    UIMoveButton = { GAMEVIEW_START_X, UI_BOTTOM_PANNEL_START_Y + TILE_SIZE, BUTTON_WIDTH, BUTTON_HEIGHT };
    UIAttackButton = { GAMEVIEW_START_X + BUTTON_WIDTH + TILE_SIZE, UI_BOTTOM_PANNEL_START_Y + TILE_SIZE, BUTTON_WIDTH, BUTTON_HEIGHT };

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

    loadTexture("C:/Users/theda/source/repos/Monolith_DND/drop_ladder.png", ladderTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/knight.png", playerTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/wall.png", wallTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/wood_door.png", doorTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/dirt_1.png", floorTexture);
    loadTexture("C:/Users/theda/source/repos/Monolith_DND/goblin.png", NPCTexture);

}

void UIManager::renderGameView(Room* currentRoom, int playerLocationX, int playerLocationY) {
    renderCurrentRoom(currentRoom);
    renderPlayer(playerLocationX, playerLocationY);  
    renderCurrentRoomNPCs(currentRoom);
};

void UIManager::renderUIPanel(SDL_Rect panel) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &panel);
};

void UIManager::renderUIButton(SDL_Rect panel) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderDrawRect(renderer, &panel);
};

void UIManager::renderUI() {
    renderUIPanel(leftUIPanel);
    renderUIPanel(rightUIPanel);
    renderUIPanel(bottomUIPanel);

    renderUIButton(UIMoveButton);
    renderUIButton(UIAttackButton);

};

void UIManager::renderPlayer(int playerLocationX, int playerLocationY) {
    SDL_Rect destRect = {
        playerLocationX,
        playerLocationY,
        TILE_SIZE,
        TILE_SIZE
    };

    SDL_RenderCopy(renderer, playerTexture, nullptr, &destRect);
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

            SDL_Rect tileRect = { (location_x * TILE_SIZE) + GAMEVIEW_START_X,
                                    (location_y * TILE_SIZE) + GAMEVIEW_START_Y,
                                    TILE_SIZE,
                                    TILE_SIZE };

            SDL_RenderCopy(renderer, NPCTexture, nullptr, &tileRect);
        }
    }
};

void UIManager::renderDarkness(int playerLocationX, int playerLocationY) {
    int gameViewWidthInTiles = GAMEVIEW_WIDTH / TILE_SIZE;
    int gameViewHeightInTiles = GAMEVIEW_HEIGHT / TILE_SIZE;

    for (int i = 0; i < gameViewWidthInTiles; i++) {
        for (int j = 0; j < gameViewHeightInTiles; j++) {

            int distanceToPLayer = findTileDistanceForShading(playerLocationX, playerLocationY, GAMEVIEW_START_X + (i * TILE_SIZE), GAMEVIEW_START_Y + (j * TILE_SIZE));
            if (distanceToPLayer > 8) {
                distanceToPLayer = 200;
            }
            else {
                distanceToPLayer *= 20;
            }

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, distanceToPLayer);
            SDL_Rect darknessOverlay = { GAMEVIEW_START_X + (i * TILE_SIZE), GAMEVIEW_START_Y + (j * TILE_SIZE), TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &darknessOverlay);
        }
    }
}

void UIManager::render(Room* currentRoom, int playerLocationX, int playerLocationY) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    renderUI();
    renderGameView(currentRoom, playerLocationX, playerLocationY);
    renderPlayer(playerLocationX, playerLocationY);

    renderDarkness(playerLocationX, playerLocationY);
};