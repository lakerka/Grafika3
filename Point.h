/* 
 * File:   Point.h
 * Author: zilva
 *
 * Created on January 7, 2015, 3:28 PM
 */

#ifndef POINT_H
#define	POINT_H

#include <GL/glut.h>
#include "Vector.h"

class Point {
    public:
        Point();
        Point(const Point& p);
        Point(GLfloat x, GLfloat y, GLfloat z);
        
        static GLfloat distance(const Point& a, const Point& b);
        
        GLfloat getX() const;
        GLfloat getY() const;
        GLfloat getZ() const;
        
        void set(GLfloat x, GLfloat y, GLfloat z);
        void set(const Point& p);
        void set(const Vector& v);
        void set(Point& p);
        
        void setX(GLfloat newX);
        void setY(GLfloat newY);
        void setZ(GLfloat newZ);
        
        Point operator+ (const Vector& v) const;
        Point operator- (const Vector& v) const;
        
    private:
        GLfloat x, y, z;
        void init(GLfloat x, GLfloat y, GLfloat z);
};

#endif	/* POINT_H */

