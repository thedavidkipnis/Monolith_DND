#pragma once
#include <string>

class Object {
private:

	int x;
	int y;

	int hitPoints;

	bool isCollectable;

	std::string description;
	std::string name;
	std::string textureID;

public:

    Object(int startX, int startY, int hitPoints, bool isCollectable, std::string description, std::string name, std::string textureID);

    int getX() const { return x; }
    void setX(int val) { x = val; }

    int getY() const { return y; }
    void setY(int val) { y = val; }

    int getHitPoints() const { return hitPoints; }
    void setHitPoints(int val) { hitPoints = val; }

    bool getIsCollectable() const { return isCollectable; }
    void setIsCollectable(bool val) { isCollectable = val; }

    const std::string& getDescription() const { return description; }
    void setDescription(const std::string& val) { description = val; }

    const std::string& getName() const { return name; }
    void setName(const std::string& val) { name = val; }

    const std::string& getTextureID() const { return textureID; }
    void setTextureID(const std::string& val) { textureID = val; }
};