/* 
 * File:   Point.cpp
 * Author: zilva
 * 
 * Created on January 7, 2015, 3:28 PM
 */

#include "Point.h"
#include "MathUtils.h"
#include <cmath>

Point::Point() {
    init(0.0, 0.0, 0.0);
}

Point::Point(const Point& p) {
    init(p.x, p.y, p.z);
}

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    init(x, y, z);
}

GLfloat Point::distance(const Point& a, const Point& b) {
    return sqrt(MathUtils::abs(MathUtils::sqr(a.getX() - b.getX())
            + MathUtils::sqr(a.getY() - b.getY())
            + MathUtils::sqr(a.getZ()- b.getZ())));
}

void Point::set(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::set(const Point& p) {
    set(p.x, p.y, p.z);
}

void Point::set(const Vector& v) {
    set(v.getX(), v.getY(), v.getZ());
}

void Point::set(Point& p) {
    set(p.x, p.y, p.z);
}

void Point::setX(GLfloat newX) {
    this->x = newX;
}

void Point::setY(GLfloat newY) {
    this->y = newY;
}

void Point::setZ(GLfloat newZ) {
    this->z = newZ;
}

void Point::init(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

GLfloat Point::getX() const {
    return x;
}

GLfloat Point::getY() const {
    return y;
}

GLfloat Point::getZ() const {
    return z;
}

Point Point::operator+ (const Vector& v) const {
    return Point (x + v.getX(), y + v.getY(), z + v.getZ());
}

Point Point::operator- (const Vector& v) const {
    return Point (x - v.getX(), y - v.getY(), z - v.getZ());
}

