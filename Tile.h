#pragma once
#include <SDL.h>
#include <SDL_image.h> 

class Tile {
private:
	int x;
	int y;

	int type;
	bool isWalkable;

    std::string textureID;

public:

    Tile(int x, int y, int type, bool walkable, std::string texture);
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getType() const { return type; }
    bool getIsWalkable() const { return isWalkable; }
    std::string getTexture() const { return textureID; }

    // Setters
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setType(int newType) { type = newType; }
    void setIsWalkable(bool walkable) { isWalkable = walkable; }
    void setTexture(std::string texture) const { texture = texture; }
};