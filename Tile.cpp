#include "Tile.h"
#include <string>

Tile::Tile(int x, int y, int type, bool walkable, std::string texture) :
	x(x), y(y), type(type), isWalkable(walkable), textureID(texture)
{}