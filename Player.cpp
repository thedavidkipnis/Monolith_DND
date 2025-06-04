#include "Player.h"
#include "Constants.h"
#include <iostream>

Player::Player() : 
    x(12), 
    y(9), 
    movementSpeed(BASE_ENTITY_SPEED_IN_TILES), 
    movementSpeedLeft(BASE_ENTITY_SPEED_IN_TILES),
    attackRange(BASE_ENTITY_ATTACK_RANGE_IN_TILES),
    damage(BASE_ENTITY_DAMAGE),
    whichDirectionIsFacing(1){}

Player::Player(int startX, int startY, int movementSpeed, int attackRange, int damage) : 
    x(startX), 
    y(startY), 
    movementSpeed(movementSpeed), 
    movementSpeedLeft(movementSpeed),
    attackRange(attackRange),
    damage(damage),
    whichDirectionIsFacing(1){}

int Player::getXTile() const {
    return (x * TILE_SIZE) + GAMEVIEW_START_X;
};
int Player::getYTile() const {
    return (y * TILE_SIZE) + GAMEVIEW_START_Y;
};

bool Player::tryMove(int deltaX, int deltaY, const Room& room) {
    int newX = x + deltaX;
    int newY = y + deltaY;

    if (room.isWalkable(newX, newY)) {
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

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

void Player::setWhichDirectionIsFacing(int direction) {
    whichDirectionIsFacing = direction;
}


bool Player::tryMoveTurnBased(int destX, int destY, const Room& room) {
    int distanceToDesiredPoint = findDistanceInTiles(x, y, destX, destY);
    return room.isWalkable(destX, destY) && distanceToDesiredPoint <= movementSpeed && distanceToDesiredPoint <= movementSpeedLeft;
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

bool Player::tryAttacking(int destX, int destY) {
    int distanceToDesiredPoint = findDistance(x, y, destX, destY);
    return distanceToDesiredPoint <= attackRange;
}

void Player::attack(int destX, int destY) {
    std::cout << "Attacked: ";
    std::cout << destX;
    std::cout << " ";
    std::cout << destY;
    std::cout << "\n";
}
