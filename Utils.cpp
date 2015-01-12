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
#include <math.h>


GLfloat Utils::degToRad(GLfloat deg) {
    return deg*PI/180.0;
}

void Utils::makeIdentity(GLfloat matrix[]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int curInd = i*4 + j;
            if (i == j) {
                matrix[curInd] = 1.0;
            }else {
                matrix[curInd] = 0.0;
            }
        }
    }
}

GLfloat Utils::sqr(GLfloat f) {
    return f*f;
}

Vector Utils::getRotatedVector(const Vector& axis, GLfloat angle, const Vector& vectorToRotate) {
    
    //rotation is perfomed twice bigger than provided angle, so we shrink the angle
    angle = angle / 2.0;
    GLfloat angleInRad = Utils::degToRad(angle);
    
    //generate quaternion q from axis and angle
    Vector normalizedAxis = axis.getNormalized(); 
    GLfloat realPart = cos(angleInRad);
    Vector imgPart = normalizedAxis*sin(angleInRad);
    Quaternion q(realPart, imgPart);

    //multiply axis by quaternion, d = qp
    Quaternion d = q*vectorToRotate;

    //find inverse q
    Quaternion invQ = q.conjugate()/Utils::sqr(q.length());

    //multiply d by inverse of q
    Quaternion rez = d*invQ;

    Vector roatatedVector(rez.getX(), rez.getY(), rez.getZ());

    return roatatedVector;
}
