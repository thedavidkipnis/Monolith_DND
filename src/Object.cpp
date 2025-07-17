#include "Object.h"
#include "Constants.h"

Object::Object(int startX, int startY, int hitPoints, bool isCollectable, std::string description, std::string name, std::string textureID) :
    x(startX),
    y(startY),
    hitPoints(hitPoints),
    isCollectable(isCollectable),
    description(description),
    name(name),
    textureID(textureID)
    {};