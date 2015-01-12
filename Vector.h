/* 
 * File:   Vector.h
 * Author: zilva
 *
 * Created on January 7, 2015, 3:31 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <iostream>
#include <GL/glut.h>	// OpenGL Graphics Utility Library

class Vector {
public:
    
    Vector();
    Vector(const Vector& orig);
    Vector (GLfloat x, GLfloat y, GLfloat z);
    virtual ~Vector();
    
    GLfloat length () const;
    bool isZero () const;
    
    //returns true if this vector == v
    bool compareWithError(Vector& v) const;

    void normalize ();
    Vector getNormalized () const;
        
    GLfloat dotProduct (const Vector& v) const;
    Vector crossProduct (const Vector& v) const;

    void set(GLfloat x, GLfloat y, GLfloat z);
    void set(const Vector& v);
    void set(Vector& v);
    
    void setX(GLfloat newX);
    void setY(GLfloat newY);
    void setZ(GLfloat newZ);
    
    GLfloat getX() const;
    GLfloat getY() const;
    GLfloat getZ() const;
    
    Vector operator+ (const Vector& v) const;
    Vector operator- (const Vector& v) const;
    Vector operator* (GLfloat s) const;
    Vector operator= (const Vector& v);
    bool operator== (const Vector& v) const;
    
private:
    GLfloat x, y, z;
};

std::ostream& operator<< (std::ostream&, const Vector&);

#endif	/* VECTOR_H */

