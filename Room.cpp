#include "Room.h"


Room::Room() : width(ROOM_WIDTH), height(ROOM_HEIGHT), isRoomAnEncounter(false), roomVisitedState(false) {
    tiles.resize(height, std::vector<int>(width, FLOOR));
}

Room::Room(int w, int h, bool isEncounter) : width(w), height(h), isRoomAnEncounter(isEncounter), roomVisitedState(false) {
    tiles.resize(height, std::vector<int>(width, FLOOR));
}

Room::Room(int w, int h, bool isEncounter, std::vector<NPC*> npcs) : width(w), height(h), isRoomAnEncounter(isEncounter), roomVisitedState(false), roomNPCs(npcs) {
    tiles.resize(height, std::vector<int>(width, FLOOR));
}

std::vector<std::vector<int>>* Room::getTiles() {
    return &tiles;
}

int Room::getTile(int x, int y) const {
    if (!isValidPosition(x, y)) return WALL;
    return tiles[y][x];
}

void Room::setTile(int x, int y, int tileType) {
    if (isValidPosition(x, y)) {
        tiles[y][x] = tileType;
    }
}

std::vector<NPC*>* Room::getListOfNPCs() {
    return &roomNPCs;
}

void Room::addNPCToRoom(int x, int y, int type) {
    NPC* newNPC = new NPC(x,y, type);
    roomNPCs.push_back(newNPC);
}

bool Room::isRoomEncounter() const {
    return isRoomAnEncounter;
};

void Room::setRoomEncounterState(bool state) {
    isRoomAnEncounter = state;
};

bool Room::hasRoomBeenVisited() const {
    return roomVisitedState;
};

void Room::setRoomVisited(bool state) {
    roomVisitedState = state;
};

bool Room::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Room::isWalkable(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    int tile = tiles[y][x];
    return tile != WALL; // Both FLOOR and DOOR are walkable
}

bool Room::isWalkableTurnBased(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    int tile = tiles[y][x];
    return (tile != WALL && tile != DOOR); // Both FLOOR and DOOR are walkable
}

void Room::generateBasicRoom() {
    generateRoomWithConnections(false, false, false, false);
}

void Room::generateRoomWithConnections(bool north, bool east, bool south, bool west) {
    // Fill with floor
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tiles[y][x] = FLOOR;
        }
    }

    addWalls();
    addDoors(north, east, south, west);
    addInteriorElements();
}

void Room::addWalls() {
    // Create walls around the perimeter
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                tiles[y][x] = WALL;
            }
        }
    }
}

void Room::addDoors(bool north, bool east, bool south, bool west) {
    int centerX = width / 2;
    int centerY = height / 2;

    if (north) {
        setTile(centerX, 0, DOOR);
    }
    if (south) {
        setTile(centerX, height - 1, DOOR);
    }
    if (east) {
        setTile(width - 1, centerY, DOOR);
    }
    if (west) {
        setTile(0, centerY, DOOR);
    }
}

bool Room::isDoor(int x, int y) const {
    return getTile(x, y) == DOOR;
}

Direction Room::getDoorDirection(int x, int y) const {
    if (!isDoor(x, y)) return NORTH; // Default, shouldn't be used

    int centerX = width / 2;
    int centerY = height / 2;

    if (y == 0 && x == centerX) return NORTH;
    if (y == height - 1 && x == centerX) return SOUTH;
    if (x == width - 1 && y == centerY) return EAST;
    if (x == 0 && y == centerY) return WEST;

    return NORTH; // Default
}

void Room::addInteriorElements() {

    // generates random wall blocks inside the room
    //std::random_device rd;
    //std::mt19937 rng(rd());

    //// Define the distributions
    //std::uniform_int_distribution<int> distA(1, 24);  // For first parameter
    //std::uniform_int_distribution<int> distB(1, 18);  // For second parameter

    //for (int i = 0; i < 5; ++i) {
    //    int a = distA(rng);
    //    int b = distB(rng);
    //    setTile(a, b, WALL);
    //}

}

void Room::processPlayerAttack(int mouseX, int mouseY, int damage) {
    for (auto it = roomNPCs.begin(); it != roomNPCs.end(); ) {
        int location_x = (*it)->getX();
        int location_y = (*it)->getY();

        if (mouseX == location_x && mouseY == location_y) { // hit
            int remainingHealth = (*it)->getHealthPoints() - damage;
            if (remainingHealth > 0) {
                (*it)->setHealthPoints(remainingHealth);
                std::cout << "Hit NPC for " << damage << " damage, " << remainingHealth << " health left.\n";
                ++it;
            }
            else {
                delete* it;
                it = roomNPCs.erase(it);
                std::cout << "NPC Died >:)\n";
            }
        }
        else {
            ++it;
        }
    }
}

void Room::processNPCAttack(int mouseX, int mouseY, int damage) {

}

void Room::processNPCActions(int playerLocationX, int playerLocationY) {
    int actionTaken = NPC_ACTION_NONE;
    for (NPC* npc : roomNPCs)
    {
        actionTaken = npc->triggerBehavior(this, playerLocationX, playerLocationY);
        switch (actionTaken) {
        case NPC_ATTACK_PLAYER:
            std::cout << "NPC ATTACKING PLAYER with " << npc->getDamage() << " damage\n";
            break;
        case NPC_MOVE:
            std::cout << "NPC MOVED\n";
            break;
        default:
            std::cout << "NO NPC ACTION TAKEN\n";
            break;
        }
    }
}