#include "Dungeon.h"
#include "Room.h"
#include "Constants.h"
#include <iostream>

Dungeon::Dungeon() : currentRoomCoord(0, 0) {
}

void Dungeon::generateInitialRooms() {
    // Create a simple 3x3 grid of rooms for testing
    // You can expand this to be more sophisticated later

    // Create center room and immediate neighbors
    std::vector<NPC*> emptyVector;
    createRoom(RoomCoord(0, 0), ROOM_WIDTH, ROOM_HEIGHT, false, emptyVector);   // Center
    createRoom(RoomCoord(0, -1), ROOM_WIDTH, ROOM_HEIGHT, false, emptyVector);  // North
    createRoom(RoomCoord(0, -2), ROOM_WIDTH, ROOM_HEIGHT, true, emptyVector);
    createRoom(RoomCoord(-1, -1), ROOM_WIDTH, ROOM_HEIGHT, false, emptyVector);  // North West
    createRoom(RoomCoord(1, 0), ROOM_WIDTH, ROOM_HEIGHT, false, emptyVector);   // East
    createRoom(RoomCoord(0, 1), ROOM_WIDTH, ROOM_HEIGHT, false, emptyVector);   // South
    createRoom(RoomCoord(-1, 0), ROOM_WIDTH, ROOM_HEIGHT, true, emptyVector);  // West

    // Setup connections for all rooms
    for (auto& [coord, room] : rooms) {
        setupRoomConnections(coord);
    }

    // create some NPCs to test rendering
    rooms[RoomCoord(0, -2)]->addNPCToRoom(10, 10, GOBLIN);
    rooms[RoomCoord(0, -2)]->addNPCToRoom(22, 2, SPIDER);

    rooms[RoomCoord(-1, 0)]->addNPCToRoom(3, 3, GOBLIN);
    rooms[RoomCoord(-1, 0)]->addNPCToRoom(19, 7, SPIDER);
    rooms[RoomCoord(-1, 0)]->addNPCToRoom(4, 7, GOBLIN);
    rooms[RoomCoord(-1, 0)]->addNPCToRoom(8, 12, SPIDER);
}

void Dungeon::createRoom(const RoomCoord& coord, int width, int height, bool isEncounter, std::vector<NPC*> npcs) {
    rooms[coord] = std::make_unique<Room>(width, height, isEncounter, npcs);
}

void Dungeon::setupRoomConnections(const RoomCoord& coord) {
    bool north = hasNeighbor(coord, NORTH);
    bool east = hasNeighbor(coord, EAST);
    bool south = hasNeighbor(coord, SOUTH);
    bool west = hasNeighbor(coord, WEST);

    rooms[coord]->generateRoomWithConnections(north, east, south, west);
}

Room* Dungeon::getCurrentRoom() {
    auto it = rooms.find(currentRoomCoord);
    return (it != rooms.end()) ? it->second.get() : nullptr;
}

Room* Dungeon::getRoom(const RoomCoord& coord) {
    auto it = rooms.find(coord);
    return (it != rooms.end()) ? it->second.get() : nullptr;
}

bool Dungeon::hasRoom(const RoomCoord& coord) const {
    return rooms.find(coord) != rooms.end();
}

bool Dungeon::hasNeighbor(const RoomCoord& coord, Direction dir) const {
    RoomCoord neighborCoord = getNeighborCoord(coord, dir);
    return hasRoom(neighborCoord);
}

RoomCoord Dungeon::getNeighborCoord(const RoomCoord& coord, Direction dir) const {
    switch (dir) {
    case NORTH: return RoomCoord(coord.x, coord.y - 1);
    case EAST:  return RoomCoord(coord.x + 1, coord.y);
    case SOUTH: return RoomCoord(coord.x, coord.y + 1);
    case WEST:  return RoomCoord(coord.x - 1, coord.y);
    default:    return coord;
    }
}

bool Dungeon::tryRoomTransition(int playerX, int playerY, int& newPlayerX, int& newPlayerY) {
    Room* currentRoom = getCurrentRoom();
    if (!currentRoom || !currentRoom->isDoor(playerX, playerY)) {
        return false;
    }

    // Get the direction of the door
    Direction doorDir = currentRoom->getDoorDirection(playerX, playerY);
    RoomCoord newRoomCoord = getNeighborCoord(currentRoomCoord, doorDir);

    // Check if the target room exists
    if (!hasRoom(newRoomCoord)) {
        return false;
    }

    // Calculate new player position in the target room
    int centerX = ROOM_WIDTH / 2;
    int centerY = ROOM_HEIGHT / 2;

    switch (doorDir) {
    case NORTH:
        newPlayerX = centerX;
        newPlayerY = ROOM_HEIGHT - 2; // Near bottom of new room
        break;
    case SOUTH:
        newPlayerX = centerX;
        newPlayerY = 1; // Near top of new room
        break;
    case EAST:
        newPlayerX = 1; // Near left of new room
        newPlayerY = centerY;
        break;
    case WEST:
        newPlayerX = ROOM_WIDTH - 2; // Near right of new room
        newPlayerY = centerY;
        break;
    }

    // Update current room
    currentRoomCoord = newRoomCoord;
    return true;
}