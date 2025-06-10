#include "Tile.h"
#include <string>

Tile::Tile(int type, bool walkable, std::string texture)
    : typeID(type), isWalkable(walkable), textureID(texture) {}