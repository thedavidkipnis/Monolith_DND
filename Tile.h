#pragma once
#include <SDL.h>
#include <SDL_image.h> 
#include <string>

class Tile {
private:

    int typeID;
    bool isWalkable;

    std::string textureID;

public:

    Tile(int type, bool walkable, std::string texture);
    // Getters
    int getType() const { return typeID; }
    bool getIsWalkable() const { return isWalkable; }
    std::string getTexture() const { return textureID; }

    // Setters
    void setType(int newType) { typeID = newType; }
    void setIsWalkable(bool walkable) { isWalkable = walkable; }
    void setTexture(const std::string& texture) { textureID = texture; }
};