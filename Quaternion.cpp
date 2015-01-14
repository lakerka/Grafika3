/* 
 * File:   Quaternion.cpp
 * Author: zilva
 * 
 * Created on January 8, 2015, 3:31 PM
 */

#include "Quaternion.h"
#include "Vector.h"
#include "Utils.h"
#include "MathUtils.h"
#include <math.h>

Quaternion::Quaternion () : r(1), x(0), y(0), z(0) {
}

Quaternion::Quaternion (const Quaternion& q) : r(q.r), x(q.x), y(q.y), z(q.z) {
}

Quaternion::Quaternion (float r, float x, float y, float z)
            : r(r), x(x), y(y), z(z) {
}

Quaternion::Quaternion (float r, Vector v) : r(r), x(v.getX()), y(v.getY()), z(v.getZ()) {
}

Quaternion::~Quaternion() {
}

Quaternion Quaternion::conjugate () const {
    return Quaternion (r, -x, -y, -z);
}

Quaternion Quaternion::negate () const {
    return Quaternion (-r, -x, -y, -z);
}

GLfloat Quaternion::norm () const {
    return x*x + y*y + z*z + r*r;
}

GLfloat Quaternion::length () const {
    return (GLfloat)sqrt (norm ());
}

Quaternion Quaternion::getNormalized () const {

    float len = length();
    if (len) {
        return Quaternion(r/len, x/len, y/len, z/len);
    }
    return Quaternion(r, x, y, z);
}

void Quaternion::normalize () {

    GLfloat len = length();
    if (len) {
        r /= len;
        x /= len;
        y /= len;
        z /= len;
    }
}

void Quaternion::toMatrix (GLfloat matrix[]) const {

    GLfloat x2 = x*x;
    GLfloat y2 = y*y;
    GLfloat z2 = z*z;
    GLfloat xy = x*y;
    GLfloat xz = x*z;
    GLfloat yz = y*z;
    GLfloat wx = r*x;
    GLfloat wy = r*y;
    GLfloat wz = r*z;

    matrix[0] = 1 - 2*y2 - 2*z2;
    matrix[1] = 2*xy + 2*wz;
    matrix[2] = 2*xz - 2*wy;

    matrix[4] = 2*xy - 2*wz;
    matrix[5] = 1 - 2*x2 - 2*z2;
    matrix[6] = 2*yz + 2*wx;

    matrix[8] = 2*xz + 2*wy;
    matrix[9] = 2*yz - 2*wx;
    matrix[10]= 1 - 2*x2 - 2*y2;
}

Quaternion Quaternion::fromAxisAndAngle(const Vector& v, GLfloat angleInDegrees) {
    angleInDegrees = angleInDegrees * 0.5f;
    GLfloat angleInRadians = MathUtils::degToRad(angleInDegrees);
    Vector normalizedVector(v);
    normalizedVector.normalize();
    GLfloat r = cos(angleInRadians);
    GLfloat s = sin(angleInRadians);
    GLfloat x = (normalizedVector.getX() * s);
    GLfloat y = (normalizedVector.getY() * s);
    GLfloat z = (normalizedVector.getZ() * s);

    return Quaternion(r, x, y, z);
}

GLfloat Quaternion::getR () const {
    return r;
}

GLfloat Quaternion::getX () const {
    return x;
}

GLfloat Quaternion::getY () const{
    return y;
}

GLfloat Quaternion::getZ () const {
    return z;
}

Quaternion Quaternion::operator+ (const Quaternion& q) const {
    return Quaternion(r + q.getR(), x + q.getX(), y + q.getY(), z + q.getZ());
}

Quaternion Quaternion::operator- (const Quaternion& q) const {
    return Quaternion(r - q.getR(), x - q.getX(), y - q.getY(), z - q.getZ());
}

Quaternion Quaternion::operator= (const Quaternion& q) {
    this->r = q.getR();
    this->x = q.getX();
    this->y = q.getY();
    this->z = q.getZ();
    return *this;
}

Quaternion Quaternion::operator/ (const GLfloat f) const {
    return Quaternion(r / f, x / f, y / f, z / f);
}

Quaternion Quaternion::operator* (const Vector& v) const {
    return Quaternion (
       -x * v.getX() - y * v.getY() - z * v.getZ(),
        r * v.getX() + y * v.getZ() - z * v.getY(),
        r * v.getY() - x * v.getZ() + z * v.getX(),
        r * v.getZ() + x * v.getY() - y * v.getX()
    );
}

Quaternion Quaternion::operator* (const Quaternion& q) const {
    return Quaternion (
        r * q.getR() - x * q.getX() - y * q.getY() - z * q.getZ(),
        r * q.getX() + x * q.getR() + y * q.getZ() - z * q.getY(),
        r * q.getY() - x * q.getZ() + y * q.getR() + z * q.getX(),
        r * q.getZ() + x * q.getY() - y * q.getX() + z * q.getR()
    );
}