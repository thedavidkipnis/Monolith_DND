#include "Player.h"
#include <iostream>

Player::Player(int startX,
    int startY,
    int movementSpeed,
    int maxHealthPoints,
    int maxInventorySize,
    int maxActionCount,
    int maxBonusActionCount,
    int attackRange,
    int damage) :
    x(startX),
    y(startY),
    movementSpeed(movementSpeed),
    movementSpeedLeft(movementSpeed),
    maxHealthPoints(maxHealthPoints),
    healthPoints(maxHealthPoints),
    maxInventorySize(maxInventorySize),
    maxActionCount(maxActionCount),
    actionCountRemaining(maxActionCount),
    maxBonusActionCount(maxBonusActionCount),
    bonusActionCountRemaining(maxBonusActionCount),
    attackRange(attackRange),
    damage(damage),
    whichDirectionIsFacing(1){}

int Player::getXTile() const {
    return (x * TILE_SIZE) + GAMEVIEW_START_X;
};
int Player::getYTile() const {
    return (y * TILE_SIZE) + GAMEVIEW_START_Y;
};

void Player::setPosition(int newX, int newY) {
    
    x = newX;
    y = newY;
}

void Player::setMovementSpeed(int sp) {
    movementSpeed = sp;
}

void Player::setMovementSpeedLeft(int sp) {
    movementSpeedLeft = sp;
}

void Player::setMaxHealthPoints(int hp) {
    maxHealthPoints = hp;
}

void Player::setHealthPoints(int hp) {
    healthPoints = hp;
}

void Player::setWhichDirectionIsFacing(int direction) {
    whichDirectionIsFacing = direction;
}

void Player::makeMoveTurnBased(int mouseX, int mouseY) {

    if (mouseX < x) { // direction setting for rendering
        whichDirectionIsFacing = LEFT;
    }
    else if (mouseX > x) {
        whichDirectionIsFacing = RIGHT;
    }

    int distanceToPoint = findDistanceInTiles(x, y, mouseX, mouseY);
    setPosition(mouseX, mouseY);
    movementSpeedLeft -= distanceToPoint;
}
