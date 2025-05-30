#include "Room.h"

Room::Room() : width(ROOM_WIDTH), height(ROOM_HEIGHT) {
    tiles.resize(height, std::vector<int>(width, FLOOR));
}

Room::Room(int w, int h) : width(w), height(h) {
    tiles.resize(height, std::vector<int>(width, FLOOR));
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

bool Room::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Room::isWalkable(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    int tile = tiles[y][x];
    return tile != WALL; // Both FLOOR and DOOR are walkable
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
    // Add some interior walls for variety
    // This will be expanded for procedural generation later
    setTile(5, 3, WALL);
    setTile(6, 3, WALL);
    setTile(5, 4, WALL);
    setTile(18, 15, WALL);
    setTile(18, 16, WALL);
    setTile(19, 15, WALL);
    setTile(8, 10, WALL);
    setTile(8, 11, WALL);
    setTile(8, 12, WALL);
}

void Room::render(SDL_Renderer* renderer) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_Rect tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            switch (tiles[y][x]) {
            case WALL:
                SDL_SetRenderDrawColor(renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case DOOR:
                SDL_SetRenderDrawColor(renderer, COLOR_BROWN.r, COLOR_BROWN.g, COLOR_BROWN.b, COLOR_BROWN.a);
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case FLOOR:
                // Floor tiles are just the black background - no drawing needed
                break;
            }
        }
    }
}