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

class Utils {
public:
    static GLfloat degToRad(GLfloat f);
    static void makeIdentity(GLfloat targetMatrix[]);
    static GLfloat sqr(GLfloat f);
    static Vector getRotatedVector(const Vector& axis, GLfloat angle, const Vector& vectorToRotate);
private:
};

#endif	/* UTILS_H */

