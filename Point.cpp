/* 
 * File:   Point.cpp
 * Author: zilva
 * 
 * Created on January 7, 2015, 3:28 PM
 */

#include "Point.h"

Point::Point() {
    init(0.0, 0.0, 0.0);
}

Point::Point(const Point& p) {
    init(p.x, p.y, p.z);
}

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    init(x, y, z);
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

