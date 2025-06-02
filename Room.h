#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Constants.h"
#include "NPC.h"
#include <string>

class Room {
private:
    std::vector<std::vector<int>> tiles;
    int width, height;

    bool isRoomAnEncounter;
    bool roomVisitedState;

    std::vector<NPC*> roomNPCs;

public:
    Room();
    Room(int w, int h, bool isRoomAnEncounter);
    Room(int w, int h, bool isRoomAnEncounter, std::vector<NPC*> npcs);

    // Tile access
    int getTile(int x, int y) const;
    void setTile(int x, int y, int tileType);
    bool isValidPosition(int x, int y) const;
    bool isWalkable(int x, int y) const;

    // NPC access
    std::vector<NPC*>* getListOfNPCs();
    void addNPCToRoom(int x, int y);

    // Room generation
    void generateBasicRoom();
    void generateRoomWithConnections(bool north, bool east, bool south, bool west);
    void addWalls();
    void addDoors(bool north, bool east, bool south, bool west);
    void addInteriorElements();

    // Door checking
    bool isDoor(int x, int y) const;
    Direction getDoorDirection(int x, int y) const;

    // Rendering
    void render(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* doorTexture) const;
    void renderRoomNPCs(SDL_Renderer* renderer, SDL_Texture* NPCTexture) const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool isRoomEncounter() const;
    void setRoomEncounterState(bool state);

    bool hasRoomBeenVisited() const;
    void setRoomVisited(bool state);
};