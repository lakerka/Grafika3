/* 
 * File:   Triangle.cpp
 * Author: zilva
 * 
 * Created on January 8, 2015, 5:08 PM
 */

#include "Triangle.h"
#include "Vector.h"

Triangle::Triangle() {
    lowerLeft.set(0.0, 0.0, 0.0);
    upperCenter.set(0.0, 0.0, 0.0);
    lowerRight.set(0.0, 0.0, 0.0);
}

Triangle::Triangle(const Triangle& orig) {
    lowerLeft.set(orig.lowerLeft);
    upperCenter.set(orig.upperCenter);
    lowerRight.set(orig.lowerRight);
}

Triangle::Triangle(const Point& lowerLeft,
        const Point& upperCenter,
        const Point& lowerRight) {
    this->lowerLeft.set(lowerLeft);
    this->upperCenter.set(upperCenter);
    this->lowerRight.set(lowerRight);
}

Triangle::~Triangle() {
}

void createVertex(const Point& p) {
    glVertex3f(p.getX(), p.getY(), p.getZ());
}

void Triangle::draw() const {
    glBegin(GL_TRIANGLES); 
    createVertex(this->lowerLeft);
    createVertex(this->upperCenter);
    createVertex(this->lowerRight);
    glEnd();
}

void Triangle::drawCylindric(GLfloat radius, GLfloat height, GLsizei slices, GLsizei stacks) const {
    
//    gluCylinder(qobj, radius, radius, height, slices, stacks);
    
//    glBegin(GL_TRIANGLES); 
//    createVertex(this->lowerLeft);
//    createVertex(this->upperCenter);
//    createVertex(this->lowerRight);
//    glEnd();
    
}

Point Triangle::getLowerLeft() const {
    return lowerLeft;
}

Point Triangle::getUpperCenter() const {
    return upperCenter;
}

Point Triangle::getLowerRight() const {
    return lowerRight;
}

void Triangle::set(float x1, float y1, float z1,
           float x2, float y2, float z2,
           float x3, float y3, float z3){
    lowerLeft.set(x1, y1, z1); 
    upperCenter.set(x2, y2, z2); 
    lowerRight.set(x3, y3, z3); 
}




