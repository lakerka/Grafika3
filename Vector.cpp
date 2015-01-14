/* 
 * File:   Vector.cpp
 * Author: zilva
 * 
 * Created on January 7, 2015, 3:31 PM
 */

#include "Vector.h"
#include "MathUtils.h"
#include "Constants.h"

#include <cmath>
#include <iostream>


Vector::Vector() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Vector::Vector (GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {
}


Vector::Vector (const Vector& orig) : x(orig.x), y(orig.y), z(orig.z) {
}

Vector::~Vector() {
}

GLfloat Vector::length () const {
    return (GLfloat)sqrt (x*x + y*y + z*z);
}

bool Vector::isZero () const {
    return x == 0 && y == 0 && z == 0;
}

Vector Vector::getNormalized() const {
    float len = length ();
    if (len) {
        return Vector (x / len, y / len, z / len);
    } else {
        return Vector (0, 0, 0);
    }
}

void Vector::normalize() {
    GLfloat len = length ();
    if (len) {
        x /= len;
        y /= len;
        z /= len;
    }
}


GLfloat Vector::getAngleInDegrees(const Vector& v1, const Vector& v2) {
    GLfloat angleInRadians = getAngleInRadians(v1, v2);
    return MathUtils::radToDeg(angleInRadians);
}

GLfloat Vector::getAngleInRadians(const Vector& v1, const Vector& v2) {
    Vector v1Copy = v1.getNormalized();
    Vector v2Copy = v2.getNormalized();
    GLfloat angleInRadians = (GLfloat)acos(v1Copy.dotProduct(v2Copy));
    if (angleInRadians > PI) {
        return 2.0*PI-angleInRadians;
    }
    return angleInRadians;
}

void Vector::set(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector::set(const Vector& v) {
    set(v.x, v.y, v.z);
}

void Vector::set(Vector& v) {
    set(v.x, v.y, v.z);
}

void Vector::setX(GLfloat newX) {
    this->x = newX;
}

void Vector::setY(GLfloat newY) {
    this->y = newY;
}

void Vector::setZ(GLfloat newZ) {
    this->z = newZ;
}

GLfloat Vector::getX() const {
    return this->x;
}

GLfloat Vector::getY() const {
    return this->y;
}

GLfloat Vector::getZ() const {
    return this->z;
}

bool Vector::compareWithError(Vector& v) const {
    GLfloat error = 0.1;
    
    GLfloat xDiff = MathUtils::abs(x - v.getX()); 
    GLfloat yDiff = MathUtils::abs(y - v.getY()); 
    GLfloat zDiff = MathUtils::abs(z - v.getZ()); 

    return xDiff < error && yDiff < error && zDiff < error;
}

Vector Vector::operator+ (const Vector& v) const {
    return Vector (x + v.getX(), y + v.getY(), z + v.getZ());
}

Vector Vector::operator- (const Vector& v) const {
    return Vector (x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator* (GLfloat s) const {
    return Vector (x * s, y * s, z * s);
}

GLfloat Vector::dotProduct (const Vector& v) const {
    return x * v.getX() + y * v.getY() + z * v.getZ();
}

Vector Vector::crossProduct (const Vector& v) const {
    return Vector (
        y*v.getZ() - z*v.getY(),
        z*v.getX() - x*v.getZ(),
        x*v.getY() - y*v.getX()
    );
}

Vector Vector::operator= (const Vector& v) {
    this->x = v.getX();
    this->y = v.getY();
    this->z = v.getZ();
    return *this;
}

bool Vector::operator== (const Vector& v) const {
    return x == v.getX() && y == v.getY() && z == v.getZ();
}