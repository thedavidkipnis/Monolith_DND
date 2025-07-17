#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "NPCBehavior.h"
#include "BehaviorWander.h"
#include "BehaviorAttackPLayerMelee.h"
#include <memory>

class NPC {
private:
    int x, y;
    int typeID;
    std::string description;
    std::string name;
    std::string textureID;
    std::string displayTextureID;

    int healthPoints;
    int movementSpeed;

    int damage;

    std::unique_ptr<NPCBehavior> behavior;

public:
    NPC(int startX, int startY, int typeID);
    ~NPC();

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    int getTypeID() const { return typeID; }
    void setTypeID(int type);

    int getHealthPoints() const { return healthPoints; }
    void setHealthPoints(int hp);

    int getMovementSpeed() const { return movementSpeed; }
    void setMovementSpeed(int ms);

    int getDamage() const { return damage; }

    std::string getDescription() const { return description; }
    void setDescription(std::string des) { description = des; }

    std::string getTextureID() const { return textureID; }
    void setTextureID(std::string texture) { textureID = texture; }

    std::string getDisplayTextureID() const { return displayTextureID; }
    void setDisplayTextureID(std::string id) { displayTextureID = id; }

    const std::string& getName() const { return name; }
    void setName(const std::string& val) { name = val; }

    // Movement
    void setPosition(int newX, int newY);

    // Logic
    int triggerBehavior(Room* room, int playerLocationX, int playerLocationY);
};