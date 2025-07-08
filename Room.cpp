#include "Room.h"


Room::Room() : 
    width(ROOM_WIDTH), height(ROOM_HEIGHT), 
    isRoomAnEncounter(false), 
    roomVisitedState(false), 
    roomNoticedOnMap(false) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, false, "none", "???")));
}

Room::Room(int w, int h, bool isEncounter) : 
    width(w), height(h), 
    isRoomAnEncounter(isEncounter), 
    roomVisitedState(false), 
    roomNoticedOnMap(false) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, false, "none", "???")));
}

Room::Room(int w, int h, bool isEncounter, std::vector<NPC*> npcs) : 
    width(w), height(h), 
    isRoomAnEncounter(isEncounter), 
    roomVisitedState(false), 
    roomNoticedOnMap(false), 
    roomNPCs(npcs) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, false, "none", "???")));
}

std::vector<std::vector<Tile>>* Room::getTiles() {
    return &Tiles;
}

Tile* Room::getTile(int x, int y) {
    if (!isValidPosition(x, y)) return nullptr;
    return &Tiles[y][x];
}

void Room::setTile(int x, int y, Tile tile) {
    if (isValidPosition(x, y)) {
        Tiles[y][x] = tile;
    }
}

void Room::createNewTile(int x, int y, int tileType) {
    Tile newTile = Tile(0, false, false, "none", "???");

    switch (tileType) {
        case WALL:
        {
            newTile.setType(WALL);
            newTile.setIsWalkable(false);
            newTile.setTexture("stone_wall");
            newTile.setTileDescription("A STURDY COBBLESTONE WALL");
            break;
        }
        case FLOOR:
        {
            newTile.setType(FLOOR);
            newTile.setIsWalkable(true);

            newTile.setTexture("dirt_" + std::to_string(getRandomIntInRange(1, 3)));
            newTile.setTileDescription("IT'S JUST DIRT...");
            break;
        }
        case TREE:
        {
            newTile.setType(TREE);
            newTile.setIsWalkable(false);
            newTile.setTexture("tree_" + std::to_string(getRandomIntInRange(1, 2)));
            newTile.setTileDescription("A GOOD OL' TREE.");
            break;
        }
        case ROCK:
            newTile.setType(ROCK);
            newTile.setIsWalkable(false);
            newTile.setTexture("rock_" + std::to_string(getRandomIntInRange(1, 2)));
            newTile.setTileDescription("IT'S JUST A ROCK...");
            break;
    }

    Tiles[x][y] = newTile;
}

std::vector<NPC*>* Room::getListOfNPCs() {
    return &roomNPCs;
}

NPC* Room::getNPCAt(int x, int y) {
    for (NPC* npc : roomNPCs) {
        if (npc->getX() == x && npc->getY() == y) {
            return npc;
        }
    }
    return nullptr;
}

void Room::addNPCToRoom(int x, int y, int type) {
    NPC* newNPC = new NPC(x,y, type);
    roomNPCs.push_back(newNPC);
    Tiles[y][x].setIsOccupied(true);
}

std::vector<Object*>* Room::getObjects() {
    return &roomObjects;
}

Object* Room::getObjectAt(int x, int y) {
    for (Object* obj : roomObjects) {
        if (obj->getX() == x && obj->getY() == y) {
            return obj;
        }
    }
    return nullptr;
}

void Room::addObjectToRoom(int x, int y, int hitPoints, bool isCollectable, std::string description, std::string name, std::string textureID) {
    Object* newObj = new Object(x, y, hitPoints, isCollectable, description, name, textureID);
    roomObjects.push_back(newObj);
    Tiles[y][x].setIsOccupied(!newObj->getIsCollectable());
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
    Tile tile = Tiles[y][x];
    return tile.getIsWalkable();
}

void Room::addDoors(bool north, bool east, bool south, bool west) {
    int centerX = width / 2;
    int centerY = height / 2;

    if (north) {
        setTile(centerX, 0, Tile(DOOR, true, false, "wood_door", "IT'S A WOODEN DOOR."));
    }
    if (south) {
        setTile(centerX, height - 1, Tile(DOOR, true, false, "wood_door", "IT'S A WOODEN DOOR."));
    }
    if (east) {
        setTile(width - 1, centerY, Tile(DOOR, true, false, "wood_door", "IT'S A WOODEN DOOR."));
    }
    if (west) {
        setTile(0, centerY, Tile(DOOR, true, false, "wood_door", "IT'S A WOODEN DOOR."));
    }
}

bool Room::isDoor(int x, int y) {
    return getTile(x, y)->getType() == DOOR;
}

Direction Room::getDoorDirection(int x, int y) {
    if (!isDoor(x, y)) return NORTH; // Default, shouldn't be used

    int centerX = width / 2;
    int centerY = height / 2;

    if (y == 0 && x == centerX) return NORTH;
    if (y == height - 1 && x == centerX) return SOUTH;
    if (x == width - 1 && y == centerY) return EAST;
    if (x == 0 && y == centerY) return WEST;

    return NORTH; // Default
}