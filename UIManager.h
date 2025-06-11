#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include "Room.h"
#include "Dungeon.h"
#include "Player.h"
#include "NPC.h"
#include "UIButton.h"
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <chrono>
#include <thread>


class UIManager {
private:
	SDL_Renderer* renderer;

    SDL_Rect gameView;
    SDL_Rect leftUIPanel;
    SDL_Rect rightUIPanel;
    SDL_Rect bottomUIPanel;

    SDL_Rect UITextBoxFrame;
    std::string currentTextBoxText;

    SDL_Rect UIMiniMapFrame;

    SDL_Rect UINPCFocusBoxFrame;
    NPC* focusedNPC;
    SDL_Rect focusedNPCFrame;

    int frameCount;

    SDL_Cursor* cursorTexture = nullptr;

    UIButton* attackButton = nullptr;
    UIButton* moveButton = nullptr;
    UIButton* endTurnButton = nullptr;

    std::vector<SDL_Texture*> playerTextures;
    std::unordered_map<std::string, SDL_Texture*> TileTextures;
    std::unordered_map<int, SDL_Texture*> NPCTextures;
    std::unordered_map<char, SDL_Texture*> AlphabetTextures;

    SDL_Rect gameOverTextureFrame;

    void loadTexture(const char* filePath, SDL_Texture*& destinationTexture);
    void loadTileTexture(const char* filePath);
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
    void renderNPCFocusBox();
    void renderPlayerStats(Player* player);
    void renderPlayer(int playerLocationX, int playerLocationY, int facingDirection);
    void renderDarkness(int playerLocationX, int playerLocationY);
    void renderCurrentRoom(Room* currentRoom);
    void renderCurrentRoomNPCs(Room* currentRoom);
    void renderCurrentRoomObjects(Room* currentRoom);
    void renderUI(Player* player);

public:

    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void render(Room* currentRoom, Player* player, int selectedPlayerAction);
    void setUITextboxText(std::string text);

    void setFocusedNPC(NPC* npc);

    void renderDeathScreen();

    void renderMap(std::map<RoomCoord, std::unique_ptr<Room>>* rooms, RoomCoord curRoomCoord, bool mapMode);

    int checkUIButtonPress(int mouseX, int mouseY);


    // animation

};