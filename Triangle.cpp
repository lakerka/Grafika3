/* 
 * File:   Triangle.cpp
 * Author: zilva
 * 
 * Created on January 8, 2015, 5:08 PM
 */

#include "Triangle.h"
#include "Vector.h"
#include "Utils.h"

#include <iostream>
using namespace std;

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

void Triangle::drawCylindric(const Point& sphereColor, const Point& cylinderColor,
        GLUquadric* qobj, 
        GLfloat cylinderRadius, GLsizei cylinderSlices, GLsizei cylinderStacks,
        GLfloat sphereRadius, GLsizei sphereSlices, GLsizei sphereStacks) const {
    
    glColor3f(cylinderColor.getX(), cylinderColor.getY(), cylinderColor.getZ());
    Utils::drawCylinder(lowerLeft, upperCenter, qobj, cylinderRadius, 
            Point::distance(lowerLeft, upperCenter), cylinderSlices, cylinderStacks);
    Utils::drawCylinder(upperCenter, lowerRight, qobj, cylinderRadius, 
            Point::distance(upperCenter, lowerRight), cylinderSlices, cylinderStacks);
    Utils::drawCylinder(lowerRight, lowerLeft, qobj, cylinderRadius, 
            Point::distance(lowerRight, lowerLeft), cylinderSlices, cylinderStacks);
    
    glColor3f(sphereColor.getX(), sphereColor.getY(), sphereColor.getZ());
    Utils::drawSphere(lowerLeft, qobj, sphereRadius, sphereSlices, sphereStacks);
    Utils::drawSphere(upperCenter, qobj, sphereRadius, sphereSlices, sphereStacks);
    Utils::drawSphere(lowerRight, qobj, sphereRadius, sphereSlices, sphereStacks); 
}

void Triangle::drawTextured() const {
    //reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    GLfloat xOffset = -0.27;
    glTexCoord2d(0.0 + xOffset, 0.0);
    createVertex(this->lowerLeft);
    glTexCoord2d(0.8 + xOffset, 1.6);
    createVertex(this->upperCenter);
    glTexCoord2d(1.6  +xOffset, 0.0);
    createVertex(this->lowerRight);
    glEnd();
    glDisable(GL_TEXTURE_2D);
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




