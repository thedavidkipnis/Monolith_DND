#pragma once
#include <SDL.h>
#include <SDL_image.h> 
#include <string>

class Tile {
private:

    int typeID;
    bool isWalkable;

    std::string textureID;
    std::string tileDescription;

public:

    Tile(int type, bool walkable, std::string texture, std::string tileDescription);
    // Getters
    int getType() const { return typeID; }
    bool getIsWalkable() const { return isWalkable; }
    std::string getTexture() const { return textureID; }
    std::string getTileDescription() const { return tileDescription; }

    // Setters
    void setType(int newType) { typeID = newType; }
    void setIsWalkable(bool walkable) { isWalkable = walkable; }
    void setTexture(const std::string& texture) { textureID = texture; }
    void setTileDescription(const std::string& description) { tileDescription = description; }
};