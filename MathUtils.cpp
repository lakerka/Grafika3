/* 
 * File:   MathUtils.cpp
 * Author: zilva
 * 
 * Created on January 14, 2015, 3:42 AM
 */

#include "MathUtils.h"

#include <math.h>
#include "Constants.h"

GLfloat MathUtils::degToRad(GLfloat deg) {
    return deg*PI/180.0;
}

GLfloat MathUtils::radToDeg(GLfloat rad) {
    return rad*180.0/PI;
}

void MathUtils::makeIdentity(GLfloat matrix[]) {
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

GLfloat MathUtils::sqr(GLfloat f) {
    return f*f;
}

GLfloat MathUtils::abs(GLfloat f) {
    if (f < 0.0) {
        return -f;    
    }
    return f;
}