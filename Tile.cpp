#include "Tile.h"
#include <string>

Tile::Tile(int type, bool walkable, std::string texture, std::string tileDescription)
    : typeID(type), isWalkable(walkable), textureID(texture), tileDescription(tileDescription) {}