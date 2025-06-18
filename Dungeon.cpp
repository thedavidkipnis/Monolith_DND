#include "Dungeon.h"
#include "Room.h"
#include "Constants.h"
#include <iostream>

// Directions
const std::vector<RoomCoord> directions = {
    {0, -1},  // North
    {1, 0},   // East
    {0, 1},   // South
    {-1, 0}   // West
};

Dungeon::Dungeon() : currentRoomCoord(0, 0) {
}

void Dungeon::generateFloorRooms(int maxRoomsOnFloor) {

    std::vector<RoomCoord> roomFrontier; // Rooms we can expand from
    std::set<RoomCoord> visited;         // Set of coordinates we’ve already placed rooms at

    RoomCoord startCoord(0, 0);
    createRoom(startCoord, ROOM_WIDTH, ROOM_HEIGHT, false);
    roomFrontier.push_back(startCoord);
    visited.insert(startCoord);

    std::random_device rd;
    std::mt19937 rng(rd());

    while (rooms.size() < maxRoomsOnFloor && !roomFrontier.empty()) {
        // Pick a random room to expand from
        std::uniform_int_distribution<int> frontierDist(0, roomFrontier.size() - 1);
        int frontierIndex = frontierDist(rng);
        RoomCoord baseCoord = roomFrontier[frontierIndex];

        // Shuffle directions to randomize which direction we try first
        std::vector<RoomCoord> shuffledDirs = directions;
        std::shuffle(shuffledDirs.begin(), shuffledDirs.end(), rng);

        bool roomCreated = false;
        for (const auto& dir : shuffledDirs) {
            RoomCoord newCoord(baseCoord.x + dir.x, baseCoord.y + dir.y);

            if (visited.count(newCoord) == 0) {
                // Create the room
                bool isEncounter = (rooms.size() >= 2 && rng() % 2 == 0); // ~50% chance for encounter after 3 rooms
                createRoom(newCoord, ROOM_WIDTH, ROOM_HEIGHT, isEncounter);
                roomFrontier.push_back(newCoord);
                visited.insert(newCoord);
                roomCreated = true;
                break; // only create one room per iteration
            }
        }

        if (!roomCreated) {
            // All neighbors were taken, remove this room from frontier
            roomFrontier.erase(roomFrontier.begin() + frontierIndex);
        }
    }


    // Setup connections for all rooms
    for (auto& [coord, room] : rooms) {
        if (room.get()->isRoomEncounter()) {
            std::string roomEncounterSpec = std::to_string(getRandomIntInRange(1, 3));
            parseAndPopulateRoomTiles(room.get(), "C:/Users/theda/source/repos/Monolith_DND/tile_mapping_encounter_0_" + roomEncounterSpec + ".csv");
            parseAndPopulateRoomNPCs(room.get(), "C:/Users/theda/source/repos/Monolith_DND/npcs_mapping_encounter_0_" + roomEncounterSpec + ".csv");
        }
        else {
            std::string randRoomSuffix = std::to_string(getRandomIntInRange(1, 3));
            parseAndPopulateRoomTiles(room.get(), "C:/Users/theda/source/repos/Monolith_DND/tile_mapping_standard_0_" + randRoomSuffix + ".csv");
        }

        setupRoomConnections(coord);

    }
}

void Dungeon::createRoom(const RoomCoord& coord, int width, int height, bool isEncounter) {
    rooms[coord] = std::make_unique<Room>(width, height, isEncounter);
}

void Dungeon::setupRoomConnections(const RoomCoord& coord) {
    bool north = hasNeighbor(coord, NORTH);
    bool east = hasNeighbor(coord, EAST);
    bool south = hasNeighbor(coord, SOUTH);
    bool west = hasNeighbor(coord, WEST);

    rooms[coord]->addDoors(north, east, south, west);
}

void Dungeon::parseAndPopulateRoomTiles(Room* room, const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    int roomIndexX = 0;
    int roomIndexY = 0;

    char c;
    while (file.get(c)) {

        if (c == '\n') {
            roomIndexX ++;
            roomIndexY = 0;
        }
        else if(c != ',' && c != ' ') // means we found one with an actual tile ID
        {
            int type = c - '0';
            room->createNewTile(roomIndexX, roomIndexY, type);

            roomIndexY++;
        }
    }

    file.close();
}
void Dungeon::parseAndPopulateRoomNPCs(Room* room, const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    int roomIndexX = 0;
    int roomIndexY = 0;

    char c;
    while (file.get(c)) {

        if (c == '\n') {
            roomIndexX++;
            roomIndexY = 0;
        }
        else if (c != ',' && c != ' ')
        {
            int ca = c - '0';

            if (ca != 0) {
                room->addNPCToRoom(roomIndexY, roomIndexX, ca);
            } 
            roomIndexY++;
        }
    }

    file.close();
}
void Dungeon::parseAndPopulateRoomObjects(Room* room, const std::string& filename) {

}

Room* Dungeon::getCurrentRoom() {
    auto it = rooms.find(currentRoomCoord);
    return (it != rooms.end()) ? it->second.get() : nullptr;
}

Room* Dungeon::getRoom(const RoomCoord& coord) {
    auto it = rooms.find(coord);
    return (it != rooms.end()) ? it->second.get() : nullptr;
}

std::map<RoomCoord, std::unique_ptr<Room>>* Dungeon::getRooms() {
    return &rooms;
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