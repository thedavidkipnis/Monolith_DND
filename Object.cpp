#include "Object.h"
#include "Constants.h"

Object::Object(int startX, int startY, int typeID) :
    x(startX),
    y(startY),
    typeID(typeID) {

    switch (typeID) {
    case BARREL:
        hitPoints = 4;
        isCollectable = false;
        description = "A STURDY BARREL.";
        name = "BARREL";
        textureID = "barrel";
        break;
    case CAMPFIRE:
        hitPoints = 6;
        isCollectable = false;
        description = "A WARM CAMPFIRE.";
        name = "CAMPRFIRE";
        textureID = "camp_fire";
        break;
    case GOLD_COINS:
        hitPoints = 4;
        isCollectable = false;
        description = "A BIT O' WEALTH.";
        name = "GOLD COINS";
        textureID = "coins";
        break;
    case CHEST:
        hitPoints = 15;
        isCollectable = false;
        description = "AN OLD CHEST.";
        name = "CHEST";
        textureID = "chest";
        break;
    default:
        hitPoints = 0;
        description = "MISSING OBJECT DESCRIPTION";
        textureID = "none";
        isCollectable = false;
        name = "NONE";
        break;
    }
};