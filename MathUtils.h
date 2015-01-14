/* 
 * File:   MathUtils.h
 * Author: zilva
 *
 * Created on January 14, 2015, 3:42 AM
 */

#ifndef MATHUTILS_H
#define	MATHUTILS_H

#include <GL/glut.h>

class MathUtils {
public:
    static GLfloat degToRad(GLfloat deg);
    static GLfloat radToDeg(GLfloat rad);
    static GLfloat sqr(GLfloat f);
    static GLfloat abs(GLfloat f);
    static void makeIdentity(GLfloat targetMatrix[]);
private:

};

#endif	/* MATHUTILS_H */

