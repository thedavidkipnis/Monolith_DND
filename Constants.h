#pragma once
#include <SDL.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <random>

// Tile system
const int TILE_SIZE = 55;
const int ROOM_WIDTH = 25;
const int ROOM_HEIGHT = 19;
const int TILE_HEIGHT_OFFSET = 0;

// Lighting stuff for rendering darkness
const int DARKNESS_SCALE = 15;
const int MAX_DARKNESS_RANGE_IN_TILES = 8;

// this variable is used by UIManager - it's the cap to the frameCount variable
// UIManager counts up to this number, then resets
const int MAX_FRAME_COUNT = 40;

// Game view
const int GAMEVIEW_WIDTH = TILE_SIZE * ROOM_WIDTH;
const int GAMEVIEW_HEIGHT = TILE_SIZE * ROOM_HEIGHT;

// Screen dimensions
const int UI_SIDE_PANEL_WIDTH = TILE_SIZE * 8;
const int UI_SIDE_PANEL_HEIGHT = TILE_SIZE * ROOM_HEIGHT;

const int SCREEN_WIDTH = GAMEVIEW_WIDTH + (2 * UI_SIDE_PANEL_WIDTH);

const int UI_BOTTOM_PANNEL_WIDTH = SCREEN_WIDTH;
const int UI_BOTTOM_PANNEL_HEIGHT = TILE_SIZE * 5;

const int SCREEN_HEIGHT = GAMEVIEW_HEIGHT + UI_BOTTOM_PANNEL_HEIGHT;

// for knowing where to start rendering game screen
const int TILE_WIDTH_OFFSET = UI_SIDE_PANEL_WIDTH;

// starting points for future layouts
const int GAMEVIEW_START_X = UI_SIDE_PANEL_WIDTH;
const int GAMEVIEW_START_Y = TILE_SIZE * 0;
const int UI_BOTTOM_PANNEL_START_X = 0;
const int UI_BOTTOM_PANNEL_START_Y = GAMEVIEW_HEIGHT;

// UI buttons and layouts
const int BUTTON_WIDTH = TILE_SIZE * 2;
const int BUTTON_HEIGHT = TILE_SIZE;

// UI Map
const int UI_MINIMAP_START_X = GAMEVIEW_START_X + GAMEVIEW_WIDTH + (0.5 * TILE_SIZE);
const int UI_MINIMAP_START_Y = 0.5 * TILE_SIZE;
const int UI_MINIMAP_WIDTH = UI_SIDE_PANEL_WIDTH - TILE_SIZE;
const int UI_MINIMAP_HEIGHT = 4 * TILE_SIZE;

// UI NPC Focus Box
const int UI_NPC_FOCUS_BOX_START_X = GAMEVIEW_START_X + GAMEVIEW_WIDTH + (0.5 * TILE_SIZE);
const int UI_NPC_FOCUS_BOX_START_Y = TILE_SIZE + UI_MINIMAP_HEIGHT;
const int UI_NPC_FOCUS_BOX_WIDTH = UI_SIDE_PANEL_WIDTH - TILE_SIZE;
const int UI_NPC_FOCUS_BOX_HEIGHT = 13 * TILE_SIZE;

// UI Textbox
const int UI_TEXTBOX_START_X = (8 * TILE_SIZE) + GAMEVIEW_START_X;
const int UI_TEXTBOX_START_Y = GAMEVIEW_HEIGHT + (0.5 * TILE_SIZE);
const int UI_TEXTBOX_WIDTH = 17 * TILE_SIZE;
const int UI_TEXTBOX_HEIGHT = UI_BOTTOM_PANNEL_HEIGHT - TILE_SIZE;

const int UI_TEXTBOX_CHAR_SIZE = 0.75 * TILE_SIZE;

enum ButtonIDs {
    NONE = 0,
    END_TURN = 1,
    MOVE = 2,
    ATTACK = 3
};

enum ButtonStates {
    INACTIVE = 0,
    ACTIVE = 1
};

// Colors
const SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };
const SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
const SDL_Color COLOR_GRAY = { 128, 128, 128, 255 };
const SDL_Color COLOR_BLUE = { 0, 100, 255, 255 };
const SDL_Color COLOR_RED = { 255, 0, 0, 255 };

// Tile types
enum TileType {
    EMPTY = 0,
    WALL = 1,
    FLOOR = 2,
    DOOR = 3,
    LADDER = 4,
    TREE = 5,
    ROCK = 6
};

// Directions for room connections
enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

// Door colors
const SDL_Color COLOR_BROWN = { 139, 69, 19, 255 };

// Entity stat constants
const int BASE_ENTITY_SPEED_IN_TILES = 5;
const int BASE_ENTITY_ATTACK_RANGE_IN_TILES = 1;
const int BASE_ENTITY_DAMAGE = 1;

// Entity list
enum facingDirectionsForRendering {
    LEFT = 0,
    RIGHT = 1
};


enum NPCTypes {
    NO_NPC = 0,
    GOBLIN = 1,
    SPIDER = 2,
    BANDIT = 3
};

enum NPCActionTypes {
    NPC_ACTION_NONE = 0,
    NPC_MOVE = 1,
    NPC_ATTACK_PLAYER = 2
};

// Math Stuff
inline int findDistanceInTiles(int x1, int y1, int x2, int y2) {

    return abs(x1 - x2) + abs(y1 - y2);

};

inline int findDistance(int x1, int y1, int x2, int y2) {

    return std::floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));

};

inline int findTileDistanceForShading(int x1, int y1, int x2, int y2) {
    if (abs(x2-x1) == abs(y2-y1)) {
        return (abs(x2 - x1) / TILE_SIZE) + 1;
    }
    return std::max(abs(x2 - x1), abs(y2 - y1)) / TILE_SIZE;
};

inline std::string extractFileName(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    std::string fileNameWithExt = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
    size_t dotPos = fileNameWithExt.find_last_of('.');
    return (dotPos == std::string::npos) ? fileNameWithExt : fileNameWithExt.substr(0, dotPos);
}

inline int getRandomIntInRange(int min, int max) {
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister RNG

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}