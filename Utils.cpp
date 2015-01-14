/* 
 * File:   Utils.cpp
 * Author: zilva
 * 
 * Created on January 10, 2015, 2:45 PM
 */

#include <GL/gl.h>

#include "Utils.h"
#include "Constants.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Point.h"
#include "Colors.h"
#include <math.h>
#include "MathUtils.h"

void Utils::drawCylinder(const Point& from, const Point& to,
        GLUquadric* qobj, GLfloat radius, GLfloat height,
        GLsizei slices, GLsizei stacks) {
    Vector fromVect(from.getX(), from.getY(), from.getZ());
    Vector toVect(to.getX(), to.getY(), to.getZ());
    Vector dir = (toVect - fromVect).getNormalized();
    Vector zDirVect(0.0, 0.0, 1.0);
    GLfloat angleInDegrees = Vector::getAngleInDegrees(dir, zDirVect);
    Vector axis = dir.crossProduct(zDirVect).getNormalized();
    
    glPushMatrix();
    glTranslatef(from.getX(), from.getY(), from.getZ());
    glRotatef(-angleInDegrees, axis.getX(), axis.getY(), axis.getZ());
    gluCylinder(qobj, radius, radius, height, slices, stacks);
    glPopMatrix();
}

void Utils::drawSphere(const Point& center,
        GLUquadric* qobj, GLfloat radius,
        GLsizei slices, GLsizei stacks) {
    glPushMatrix();
    glTranslatef(center.getX(), center.getY(), center.getZ());
    gluSphere(qobj, radius, slices, stacks);
    glPopMatrix();
}

Vector Utils::getRotatedVector(const Vector& axis, GLfloat angle, const Vector& vectorToRotate) {
    
    //rotation is perfomed twice bigger than provided angle, so we shrink the angle
    angle = angle / 2.0;
    GLfloat angleInRad = MathUtils::degToRad(angle);
    
    //generate quaternion q from axis and angle
    Vector normalizedAxis = axis.getNormalized(); 
    GLfloat realPart = cos(angleInRad);
    Vector imgPart = normalizedAxis*sin(angleInRad);
    Quaternion q(realPart, imgPart);

    //multiply axis by quaternion, d = qp
    Quaternion d = q*vectorToRotate;

    //find inverse q
    Quaternion invQ = q.conjugate()/MathUtils::sqr(q.length());

    //multiply d by inverse of q
    Quaternion rez = d*invQ;

    Vector roatatedVector(rez.getX(), rez.getY(), rez.getZ());

    return roatatedVector;
}

void Utils::calculatePlane(GLfloat targetPlane[], const Point& p1, const Point& p2, const Point& p3) {
       
    targetPlane[0] = (((p2.getY() - p1.getY())*(p3.getZ() - p1.getZ())) - ((p2.getZ() - p1.getZ())
            * (p3.getY() - p1.getY())));
    targetPlane[1] = (((p2.getZ() - p1.getZ())*(p3.getX() - p1.getX())) - ((p2.getX() - p1.getX())
            * (p3.getZ() - p1.getZ())));
    targetPlane[2] = (((p2.getX() - p1.getX())*(p3.getY() - p1.getY())) - ((p2.getY() - p1.getY())
            * (p3.getX() - p1.getX())));
    targetPlane[3] = -(targetPlane[0]*p1.getX() + targetPlane[1]*p1.getY() + targetPlane[2]*p1.getZ());
}