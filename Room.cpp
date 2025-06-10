#include "Room.h"


Room::Room() : width(ROOM_WIDTH), height(ROOM_HEIGHT), isRoomAnEncounter(false), roomVisitedState(false) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, "none")));
}

Room::Room(int w, int h, bool isEncounter) : width(w), height(h), isRoomAnEncounter(isEncounter), roomVisitedState(false) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, "none")));
}

Room::Room(int w, int h, bool isEncounter, std::vector<NPC*> npcs) : width(w), height(h), isRoomAnEncounter(isEncounter), roomVisitedState(false), roomNPCs(npcs) {
    Tiles.resize(height, std::vector<Tile>(width, Tile(0, false, "none")));
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
    Tile tile = Tiles[y][x];
    return tile.getIsWalkable(); // Both FLOOR and DOOR are walkable
}

bool Room::isWalkableTurnBased(int startX, int startY, int x, int y, int availableDistance) const {
    if (!isValidPosition(x, y)) return false;
    Tile tile = Tiles[y][x];
    return (tile.getIsWalkable() && (findDistanceInTiles(startX, startY, x, y) <= availableDistance));
}

void Room::addDoors(bool north, bool east, bool south, bool west) {
    int centerX = width / 2;
    int centerY = height / 2;

    if (north) {
        setTile(centerX, 0, Tile(DOOR, true, "wood_door"));
    }
    if (south) {
        setTile(centerX, height - 1, Tile(DOOR, true, "wood_door"));
    }
    if (east) {
        setTile(width - 1, centerY, Tile(DOOR, true, "wood_door"));
    }
    if (west) {
        setTile(0, centerY, Tile(DOOR, true, "wood_door"));
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

void Room::processNPCActions(Player* player) {
    int actionTaken = NPC_ACTION_NONE;
    for (NPC* npc : roomNPCs)
    {
        actionTaken = npc->triggerBehavior(this, player->getX(), player->getY());
        switch (actionTaken) {
        case NPC_ATTACK_PLAYER:
            std::cout << "NPC ATTACKING PLAYER with " << npc->getDamage() << " damage\n";
            player->setHealthPoints(player->getHealthPoints() - npc->getDamage());
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