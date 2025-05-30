#pragma once
#include <SDL.h>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Tile system
const int TILE_SIZE = 32;
const int ROOM_WIDTH = 25;
const int ROOM_HEIGHT = 19;

// Colors
const SDL_Color COLOR_BLACK = { 0, 0, 0, 255 };
const SDL_Color COLOR_WHITE = { 255, 255, 255, 255 };
const SDL_Color COLOR_GRAY = { 128, 128, 128, 255 };
const SDL_Color COLOR_BLUE = { 0, 100, 255, 255 };

// Tile types
enum TileType {
    EMPTY = 0,
    WALL = 1,
    FLOOR = 2,
    DOOR = 3
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