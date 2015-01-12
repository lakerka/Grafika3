/* 
 * File:   Camera.cpp
 * Author: zilva
 * 
 * Created on January 10, 2015, 4:35 PM
 */

#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera() {
    this->position = Point(0.0, 0.0, 0.0);
    this->facingDirection = Vector(0.0, 0.0, 1.0);
    this->up = Vector(0.0, 1.0, 0.0);
}

Camera::Camera(const Camera& orig) {
    this->position = Point(orig.getPosition());
    this->facingDirection = orig.getFacingDirection();
}

Camera::~Camera() {
}

void Camera::setPosition(const Point& newPosition) {
    this->position = Point(newPosition);
}

void Camera::setFacingDirection(const Vector& newFacingDirection) {
    this->facingDirection = newFacingDirection;
}

void Camera::setUp(const Vector& newUp) {
    this->up = newUp;
}

Point Camera::getPosition() const {
    return this->position;
}

Vector Camera::getFacingDirection() const {
    return this->facingDirection;
}

Vector Camera::getUp() const {
    return this->up;
}

