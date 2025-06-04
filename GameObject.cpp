#include "GameObject.h"
#include "Constants.h"
#include <iostream>

GameObject::GameObject(int startX, int startY) : x(startX), y(startY) {
};

GameObject::~GameObject() {
};

// Setters
void GameObject::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
};