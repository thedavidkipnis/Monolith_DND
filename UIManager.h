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

    SDL_Rect UITextBoxFrame;
    std::string currentTextBoxText;

    int frameCount;

    SDL_Cursor* cursorTexture = nullptr;

    UIButton* attackButton = nullptr;
    UIButton* moveButton = nullptr;
    UIButton* endTurnButton = nullptr;

    std::vector<SDL_Texture*> playerTextures;
    SDL_Texture* NPCTexture = nullptr;

    std::unordered_map<int, SDL_Texture*> NPCTextures;
    std::unordered_map<char, SDL_Texture*> AlphabetTextures;

    SDL_Texture* wallTexture = nullptr;
    SDL_Texture* ladderTexture = nullptr;
    SDL_Texture* floorTexture = nullptr;
    SDL_Texture* doorTexture = nullptr;

    SDL_Texture* playerHealthHeartTexture;
    SDL_Texture* playerHealthHalfHeartTexture;

    SDL_Texture* playerMovementSpeedTexture;

    SDL_Rect gameOverTextureFrame;
    SDL_Texture* gameOverTexture;

    void loadTexture(const char* filePath, SDL_Texture*& destinationTexture);
    void loadTextures();
    void loadPlayerTextures();
    void loadAlphabetTextures();
    void loadNPCTextures();
    void loadUIButtons();
    void renderGameView(Room* currentRoom, Player* player);
    void renderUIPanel(SDL_Rect panel);
    void renderUIButton(UIButton* button);
    void updateUIButtonsBasedOnSelectedAction(int selectedPlayerAction);
    void renderUITextBox();
    void renderPlayerStats(int healthPoints, int movementSpeed);
    void renderPlayer(int playerLocationX, int playerLocationY, int facingDirection);
    void renderDarkness(int playerLocationX, int playerLocationY);
    void renderCurrentRoom(Room* currentRoom);
    void renderCurrentRoomNPCs(Room* currentRoom);
    void renderCurrentRoomObjects(Room* currentRoom);
    void renderActionableTiles(int playerX, int playerY);
    void renderUI(int playerHP, int playerMP);

public:

    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void render(Room* currentRoom, Player* player, int selectedPlayerAction);
    void setUITextboxText(std::string text);

    void renderDeathScreen();

    int checkUIButtonPress(int mouseX, int mouseY);
    void toggleButton(int button);

};