#include "Tile.h"
#include <string>

Tile::Tile(int type, bool walkable, bool isOccupied, std::string texture, std::string tileDescription)
    : typeID(type), 
    isWalkable(walkable), 
    isOccupied(isOccupied),
    textureID(texture), 
    tileDescription(tileDescription) {}