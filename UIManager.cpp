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

void UIManager::loadTextures() {
    SDL_Surface* surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/knight.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!playerTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    surface = IMG_Load("C:/Users/theda/source/repos/Monolith_DND/wall.png");
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

void UIManager::renderGameView(Room* currentRoom) {
    if (currentRoom) {
        currentRoom->render(renderer, wallTexture, doorTexture);
        currentRoom->renderRoomNPCs(renderer, NPCTexture);
    }
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
        (playerLocationX * TILE_SIZE) + GAMEVIEW_START_X,
        (playerLocationY * TILE_SIZE) + GAMEVIEW_START_Y,
        TILE_SIZE,
        TILE_SIZE
    };

    SDL_RenderCopy(renderer, playerTexture, nullptr, &destRect);
}

void UIManager::render(Room* currentRoom, int playerLocationX, int playerLocationY) {
    SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(renderer);

    renderUI();
    renderGameView(currentRoom);
    renderPlayer(playerLocationX, playerLocationY);
};