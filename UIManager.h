#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include "Room.h"
#include "Player.h"
#include "UIButton.h"
#include <iostream>


class UIManager {
private:
	SDL_Renderer* renderer;

    std::unique_ptr<Player> player;

    SDL_Rect gameView;
    SDL_Rect leftUIPanel;
    SDL_Rect rightUIPanel;
    SDL_Rect bottomUIPanel;

    SDL_Rect UIEndTurnButton;

    SDL_Cursor* cursorTexture = nullptr;

    UIButton* attackButton = nullptr;
    UIButton* moveButton = nullptr;
    UIButton* endTurnButton = nullptr;

    SDL_Texture* endTurnButtonPressedTexture = nullptr;
    SDL_Texture* endTurnButtonTexture = nullptr;

    SDL_Texture* playerTexture = nullptr;
    SDL_Texture* NPCTexture = nullptr;

    SDL_Texture* wallTexture = nullptr;
    SDL_Texture* ladderTexture = nullptr;
    SDL_Texture* floorTexture = nullptr;
    SDL_Texture* doorTexture = nullptr;

    void loadTexture(const char* filePath, SDL_Texture*& destinationTexture);
    void loadTextures();
    void loadUIButtons();
    void renderGameView(Room* currentRoom, int playerLocationX, int playerLocationY);
    void renderUIPanel(SDL_Rect panel);
    void renderUIButton(UIButton* button);
    void renderPlayer(int playerLocationX, int playerLocationY);
    void renderDarkness(int playerLocationX, int playerLocationY);
    void renderCurrentRoom(Room* currentRoom);
    void renderCurrentRoomNPCs(Room* currentRoom);
    void renderUI();

public:

    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void render(Room* currentRoom, int playerLocationX, int playerLocationY);
    int checkUIButtonPress(int mouseX, int mouseY);

};