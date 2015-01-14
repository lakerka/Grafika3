/* 
 * File:   Triangle.h
 * Author: zilva
 *
 * Created on January 8, 2015, 5:08 PM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include "Point.h"
#include <GL/glut.h>	// OpenGL Graphics Utility Library

class Triangle {
public:
    Triangle();
    
    Triangle(const Triangle& orig);

    Triangle (const Point& lowerLeft,
            const Point& upperCenter,
            const Point& lowerRight);
    
    Triangle (GLfloat x1, GLfloat y1, GLfloat z1,
             GLfloat x2, GLfloat y2, GLfloat z2,
             GLfloat x3, GLfloat y3, GLfloat z3);
    
    virtual ~Triangle();
    
    void draw() const;
    void drawCylindric(const Point& sphereColor, const Point& cylinderColor,
        GLUquadric* qobj, 
        GLfloat cylinderRadius, GLsizei cylinderSlices, GLsizei cylinderStacks,
        GLfloat sphereRadius, GLsizei sphereSlices, GLsizei sphereStacks) const;
    void drawTextured() const;
    
    Point getLowerLeft() const;
    Point getUpperCenter() const;
    Point getLowerRight() const;
    
    void set(GLfloat x1, GLfloat y1, GLfloat z1,
            GLfloat x2, GLfloat y2, GLfloat z2,
            GLfloat x3, GLfloat y3, GLfloat z3);
    
private:
    Point lowerLeft, upperCenter, lowerRight;
};

#endif	/* TRIANGLE_H */

