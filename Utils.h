/* 
 * File:   Utils.h
 * Author: zilva
 *
 * Created on January 10, 2015, 2:45 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <GL/glut.h>	// OpenGL Graphics Utility Library
#include "Vector.h"
#include "Point.h"

class Utils {
public:
    static Vector getRotatedVector(const Vector& axis, GLfloat angle, const Vector& vectorToRotate);
    static void calculatePlane(GLfloat targetPlane[], const Point& p1, const Point& p2, const Point& p3);
    static void drawCylinder(const Point& from, const Point& to,
        GLUquadric* qobj, GLfloat radius, GLfloat height,
        GLsizei slices, GLsizei stacks);
    static void drawSphere(const Point& center,
        GLUquadric* qobj, GLfloat radius,
        GLsizei slices, GLsizei stacks);
private:
};

#endif	/* UTILS_H */

