#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include "Room.h"
#include "Player.h"
#include <iostream>


class UIManager {
private:
	SDL_Renderer* renderer;

    std::unique_ptr<Player> player;

    SDL_Rect gameView;
    SDL_Rect leftUIPanel;
    SDL_Rect rightUIPanel;
    SDL_Rect bottomUIPanel;

    SDL_Rect UIAttackButton;
    SDL_Rect UIMoveButton;

    SDL_Cursor* cursorTexture = nullptr;

    SDL_Texture* ladderTexture = nullptr;

    SDL_Texture* playerTexture = nullptr;
    SDL_Texture* wallTexture = nullptr;
    SDL_Texture* floorTexture = nullptr;
    SDL_Texture* doorTexture = nullptr;
    SDL_Texture* NPCTexture = nullptr;

    void loadTexture(const char* filePath, SDL_Texture*& destinationTexture);
    void loadTextures();
    void renderGameView(Room* currentRoom, int playerLocationX, int playerLocationY);
    void renderUIPanel(SDL_Rect panel);
    void renderUIButton(SDL_Rect panel);
    void renderPlayer(int playerLocationX, int playerLocationY);
    void renderDarkness(int playerLocationX, int playerLocationY);
    void renderCurrentRoom(Room* currentRoom);
    void renderCurrentRoomNPCs(Room* currentRoom);
    void renderUI();

public:

    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void render(Room* currentRoom, int playerLocationX, int playerLocationY);

};