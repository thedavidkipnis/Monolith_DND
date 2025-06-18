#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include <string>


class Player {
private:
    int x, y;

    int movementSpeed;
    int movementSpeedLeft;

    int maxHealthPoints;
    int healthPoints;

    int maxActionCount;
    int actionCountRemaining;
    int maxBonusActionCount;
    int bonusActionCountRemaining;

    int attackRange;
    int damage;

    int whichDirectionIsFacing;

public:
    Player(
        int startX, 
        int startY, 
        int movementSpeed, 
        int maxHealthPoints, 
        int maxActionCount,
        int maxBonusActionCount,
        int attackRange, 
        int damage);

    int getX() const { return x; }
    int getY() const { return y; }

    int getXTile() const;
    int getYTile() const;

    void setPosition(int newX, int newY);

    int getMovementSpeed() const { return movementSpeed; }
    int getMovementSpeedLeft() const { return movementSpeedLeft; }

    void setMovementSpeed(int sp);
    void setMovementSpeedLeft(int sp);

    int getMaxHealthPoints() const { return maxHealthPoints; }
    void setMaxHealthPoints(int hp);

    int getHealthPoints() const { return healthPoints; }
    void setHealthPoints(int hp);

    int getMaxActionCount() const { return maxActionCount; }
    void setMaxActionCount(int val) { maxActionCount = val; }

    int getActionCountRemaining() const { return actionCountRemaining; }
    void setActionCountRemaining(int val) { actionCountRemaining = val; }

    int getMaxBonusActionCount() const { return maxBonusActionCount; }
    void setMaxBonusActionCount(int val) { maxBonusActionCount = val; }

    int getBonusActionCountRemaining() const { return bonusActionCountRemaining; }
    void setBonusActionCountRemaining(int val) { bonusActionCountRemaining = val; }

    int getDamage() const { return damage; }
    int getAttackRange() const { return attackRange; }

    int getWhichDirectionIsFacing() const { return whichDirectionIsFacing; }
    void setWhichDirectionIsFacing(int direction);

    void makeMoveTurnBased(int mouseX, int mouseY);

};