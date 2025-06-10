#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include "Tile.h"
#include "Constants.h"
#include "Player.h"
#include "NPC.h"

class Room {
private:
    int width, height;

    std::vector<std::vector<Tile>> Tiles;

    bool isRoomAnEncounter;
    bool roomVisitedState;

    std::vector<NPC*> roomNPCs;

public:
    Room();
    Room(int w, int h, bool isRoomAnEncounter);
    Room(int w, int h, bool isRoomAnEncounter, std::vector<NPC*> npcs);

    // Tile access
    std::vector<std::vector<Tile>>* getTiles();
    Tile* getTile(int x, int y) ;
    void setTile(int x, int y, Tile tile);
    void createNewTile(int x, int y, int tileType);
    bool isValidPosition(int x, int y) const;
    bool isWalkable(int x, int y) const;
    bool isWalkableTurnBased(int startX, int startY, int x, int y, int availableDistance) const;

    // NPC access
    std::vector<NPC*>* getListOfNPCs();
    void addNPCToRoom(int x, int y, int type);

    // Room generation
    void addDoors(bool north, bool east, bool south, bool west);

    // Door checking
    bool isDoor(int x, int y);
    Direction getDoorDirection(int x, int y);

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool isRoomEncounter() const;
    void setRoomEncounterState(bool state);

    bool hasRoomBeenVisited() const;
    void setRoomVisited(bool state);

    // Player interaction
    void processPlayerAttack(int mouseX, int mouseY, int damage);
};