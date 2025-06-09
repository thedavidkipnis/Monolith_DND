#pragma once
#include <SDL.h>
#include <memory>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "Constants.h"
#include "NPC.h"

class Room;

struct RoomCoord {
    int x, y;

    RoomCoord(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const RoomCoord& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const RoomCoord& other) const {
        return x == other.x && y == other.y;
    }
};

class Dungeon {
private:
    std::map<RoomCoord, std::unique_ptr<Room>> rooms;
    RoomCoord currentRoomCoord;

public:
    Dungeon();
    ~Dungeon() = default;

    // Room management
    void generateFloorRooms(int maxRoomsOnFloor);
    Room* getCurrentRoom();
    Room* getRoom(const RoomCoord& coord);
    bool hasRoom(const RoomCoord& coord) const;

    std::map<RoomCoord, std::unique_ptr<Room>>* getRooms();

    RoomCoord getCurRoomCoord() const { return currentRoomCoord; }

    // Room transitions
    bool tryRoomTransition(int playerX, int playerY, int& newPlayerX, int& newPlayerY);

    // Room connections
    bool hasNeighbor(const RoomCoord& coord, Direction dir) const;
    RoomCoord getNeighborCoord(const RoomCoord& coord, Direction dir) const;

private:
    void createRoom(const RoomCoord& coord, int width, int height, bool isEncounter);
    void parseAndPopulateRoomTiles(Room* room, const std::string& filename);
    void parseAndPopulateRoomNPCs(Room* room, const std::string& filename);
    void parseAndPopulateRoomObjects(Room* room, const std::string& filename);
    void setupRoomConnections(const RoomCoord& coord);
};
