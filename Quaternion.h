/* 
 * File:   Quaternion.h
 * Author: zilva
 *
 * Created on January 7, 2015, 3:31 PM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H

#include <iostream>
#include "Vector.h"
#include <GL/glut.h>	// OpenGL Graphics Utility Library

class Quaternion {
public:
    Quaternion();
    Quaternion(const Quaternion& orig);
    Quaternion (GLfloat r, GLfloat x, GLfloat y, GLfloat z);
    Quaternion (GLfloat r, Vector v);
    virtual ~Quaternion();

    Quaternion conjugate () const;
    Quaternion negate () const;
    GLfloat norm () const;
    GLfloat length () const;
    void normalize();
    Quaternion getNormalized () const;
    
    void toMatrix (GLfloat targetMatrix[]) const;
    static Quaternion fromAxisAndAngle(const Vector& v, float angleInDegrees);
    
    GLfloat getR() const;
    GLfloat getX() const;
    GLfloat getY() const;
    GLfloat getZ() const;
    
    Quaternion operator+ (const Quaternion& q) const;
    Quaternion operator- (const Quaternion& q) const;
    Quaternion operator= (const Quaternion& q);
    Quaternion operator/ (const float f) const;
    Quaternion operator* (const Vector& v) const;
    Quaternion operator* (const Quaternion& q) const;
private:
    GLfloat r, x, y, z;
};

std::ostream& operator<< (std::ostream&, const Quaternion&);

#endif	/* QUATERNION_H */

