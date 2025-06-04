#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include "Room.h"
#include "Player.h"
#include "UIButton.h"
#include <unordered_map>
#include <iostream>
#include <cmath>


class UIManager {
private:
	SDL_Renderer* renderer;

    SDL_Rect gameView;
    SDL_Rect leftUIPanel;
    SDL_Rect rightUIPanel;
    SDL_Rect bottomUIPanel;

    int frameCount;

    SDL_Cursor* cursorTexture = nullptr;

    UIButton* attackButton = nullptr;
    UIButton* moveButton = nullptr;
    UIButton* endTurnButton = nullptr;

    SDL_Texture* playerTexture = nullptr;
    SDL_Texture* NPCTexture = nullptr;

    std::unordered_map<int, SDL_Texture*> NPCTextures;

    SDL_Texture* wallTexture = nullptr;
    SDL_Texture* ladderTexture = nullptr;
    SDL_Texture* floorTexture = nullptr;
    SDL_Texture* doorTexture = nullptr;

    void loadTexture(const char* filePath, SDL_Texture*& destinationTexture);
    void loadTextures();
    void loadNPCTextures();
    void loadUIButtons();
    void renderGameView(Room* currentRoom, Player* player);
    void renderUIPanel(SDL_Rect panel);
    void renderUIButton(UIButton* button);
    void renderPlayer(int playerLocationX, int playerLocationY, int facingDirection);
    void renderDarkness(int playerLocationX, int playerLocationY);
    void renderCurrentRoom(Room* currentRoom);
    void renderCurrentRoomNPCs(Room* currentRoom);
    void renderCurrentRoomObjects(Room* currentRoom);
    void renderActionableTiles(int playerX, int playerY);
    void renderUI();

public:

    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void render(Room* currentRoom, Player* player);
    int checkUIButtonPress(int mouseX, int mouseY);

};