#include "Player.h"
#include "Constants.h"
#include <iostream>

Player::Player() : 
    x(12), 
    y(9), 
    movementSpeed(BASE_ENTITY_SPEED_IN_TILES), 
    movementSpeedLeft(BASE_ENTITY_SPEED_IN_TILES),
    attackRange(BASE_ENTITY_ATTACK_RANGE_IN_TILES),
    damage(BASE_ENTITY_DAMAGE){}

Player::Player(int startX, int startY, int movementSpeed, int attackRange, int damage) : 
    x(startX), 
    y(startY), 
    movementSpeed(movementSpeed), 
    movementSpeedLeft(movementSpeed),
    attackRange(attackRange),
    damage(damage){} 

Player::~Player() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
    }
}

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

bool Player::tryMoveTurnBased(int destX, int destY, const Room& room) {
    int distanceToDesiredPoint = findDistanceInTiles(x, y, destX, destY);
    return room.isWalkable(destX, destY) && distanceToDesiredPoint <= movementSpeed && distanceToDesiredPoint <= movementSpeedLeft;
}

void Player::makeMoveTurnBased(int mouseX, int mouseY) {
    int distanceToPoint = findDistanceInTiles(x, y, mouseX, mouseY);
    setPosition(mouseX, mouseY);
    movementSpeedLeft -= distanceToPoint;
}

bool Player::tryAttacking(int destX, int destY) {
    int distanceToDesiredPoint = findDistanceInTiles(x, y, destX, destY);
    return distanceToDesiredPoint <= attackRange;
}

void Player::attack(int destX, int destY) {
    std::cout << "Attacked: ";
    std::cout << destX;
    std::cout << " ";
    std::cout << destY;
    std::cout << "\n";
}
