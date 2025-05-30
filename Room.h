#pragma once
#include <SDL.h>
#include <vector>
#include "Constants.h"

class Room {
private:
    std::vector<std::vector<int>> tiles;
    int width, height;

public:
    Room();
    Room(int w, int h);

    // Tile access
    int getTile(int x, int y) const;
    void setTile(int x, int y, int tileType);
    bool isValidPosition(int x, int y) const;
    bool isWalkable(int x, int y) const;

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
    void render(SDL_Renderer* renderer) const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};