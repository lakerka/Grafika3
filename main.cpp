/* 
 * File:   main.cpp
 * Author: zilva
 *
 * Created on January 7, 2015, 3:19 PM
 */

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>   
#include "Constants.h"
#include "MathUtils.h"
#include "Utils.h"
#include "Point.h"
#include "Triangle.h"
#include "Colors.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Camera.h"
#include "MathUtils.h"
#include <cmath>	
#include <GL/glut.h>
#include <SOIL/SOIL.h>

using namespace std;

enum Direction { LEFT, RIGHT, UP, DOWN};

const GLint CYLINDRIC_TYPE = 1;
const GLint TEXTURED_TYPE = 2;

Point lights[3];
Vector lightsDir[3];
        
int selectedLight = 0;

GLsizei windowW, windowH;

/*movement and camera related*/
char moveLeftKey = 'a';
char moveRightKey = 'd';
char moveForwardKey = 'w';
char moveBackKey = 's';

char moveLightUpKey = '8';
char moveLightDownKey = '2';
char moveLightLeftKey = '4';
char moveLightRightKey = '6';

Camera eyeCamera;
int keyState[500];
GLfloat facingHAngle, facingVAngle; 
GLfloat movingSpeed = MOVING_SPEED;

/*cylinder related*/
GLUquadric* qobj;
Point sphereColor(red);
GLfloat sphereRadius = 0.2;
GLint sphereSlices = 10;
GLint sphereSlacks = 10;

Point cylinderColor(peachpuff);
GLfloat cylinderRadius = 0.1;
GLint cylinderSlices = 10;
GLint cylinderSlacks = 10;

Point cylindricBallCenter(7.0, 3.0, 13.0);
Point texturedBallCenter(13.0, 3.0, 7.0);

/*texture related*/
GLuint textureId;
GLenum DEFAULT_TEXTURE_WRAP = GL_CLAMP;

/*other*/
GLfloat aspect = 1.0;
GLfloat zOffset = 0.0;

static void print(const char* str) {
    fprintf(stderr, "%s\n", str);
}

static void print(char* str) {
    fprintf(stderr, "%s\n", str);
}

static void printMaxtrix(GLfloat mat[]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int ind = i*4 + j;
            if (j  != 0) {
                cout << ",";
            }
            cout << " " << mat[ind]; 
            //printf("%d ", ind);
        }
        cout << endl;
    }
}

static void printMaxtrix(GLfloat mat[][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if (j  != 0) {
                cout << ",";
            }
            cout << " " << mat[i][j];
        }
        cout << endl;
    }
}

ostream& operator<< (ostream& os, const Quaternion& q) {
    os << "===== Quaternion =====\n"
        << "(" << q.getR()
        << " + " << q.getX() << "i"
        << " + " << q.getY() << "j"
        << " + " << q.getZ() << "k)";
    return os;
}

ostream& operator<< (ostream& os, const Vector& v) {
    os << "===== Vector =====\n"
        << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    return os;
}

ostream& operator<< (ostream& os, const Point& p) {
    os << "===== Point =====\n"
        << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
    return os;
}


void drawAxis(const Vector& v, GLfloat lineLen) {
    Vector normalizedVect(v);
    normalizedVect.normalize();
    Vector arrow(normalizedVect*lineLen);

    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(arrow.getX(), arrow.getY(), arrow.getZ());
    glEnd();
}

void drawXYZAxis(GLfloat lineLen) {
    glLineWidth(5.0);
    glColor3f(green);
    drawAxis(Vector(1.0, 0.0, 0.0), lineLen);
    glColor3f(red);
    drawAxis(Vector(0.0, 1.0, 0.0), lineLen);
    glColor3f(cayan); 
    drawAxis(Vector(0.0, 0.0, 1.0), lineLen);
}


void drawHorizontalSlab(GLfloat startX, GLfloat y, GLfloat startZ, GLfloat slabW, 
        GLfloat slabH) {
    glBegin(GL_QUADS);
        glVertex3f(startX, y, startZ);
        glVertex3f(startX + slabW, y, startZ);
        glVertex3f(startX + slabW, y, startZ + slabH);
        glVertex3f(startX, y, startZ + slabH);
    glEnd();
}

void drawVerticalSlab(GLfloat startX, GLfloat startY, GLfloat startZ, GLfloat slabW, 
        GLfloat slabH, bool isAlongX) {
    glBegin(GL_QUADS);
        glVertex3f(startX, startY, startZ);
        glVertex3f(startX, startY + slabH, startZ);
        glVertex3f(startX + slabW*isAlongX, startY + slabH, startZ + slabW*(!isAlongX));
        glVertex3f(startX + slabW*isAlongX, startY, startZ + slabW*(!isAlongX));
    glEnd();
}

void drawFloor(GLfloat startX, GLfloat y, GLfloat startZ, GLsizei width, 
        GLsizei height, GLfloat slabW, GLfloat slabH) {
    bool isWhite = 0;
    bool rowFirstIsWhite = 0;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            isWhite = !isWhite;
            if (j == 0) {
                isWhite = rowFirstIsWhite;
                rowFirstIsWhite = !rowFirstIsWhite;
            }
            if (isWhite) {
                glColor3f(white);
            }else {
                glColor3f(black);
            }
            drawHorizontalSlab(startX + i*slabW, y, startZ + j*slabH, slabW, slabH); 
        }
    }
}

void drawWalls(GLfloat startX, GLfloat startY, GLfloat startZ, GLsizei width, 
        GLsizei height) {
    glColor3f(lightGrey2);
    drawVerticalSlab(startX, startY, startZ, width, height, true);
    glColor3f(lightGrey1);
    drawVerticalSlab(startX, startY, startZ, width, height, false);
}

void drawDirectionAndUpAxis(const Camera& camera, GLfloat lineLen) {
    Vector facingDir = camera.getFacingDirection();
    Vector up = camera.getUp();
    glLineWidth(5.0);
    glColor3f(green);
    drawAxis(Vector(facingDir.getX(), facingDir.getY(), facingDir.getZ()), lineLen);
    glColor3f(red);
    drawAxis(Vector(up.getX(), up.getY(), up.getZ()), lineLen);
}


void turnCamera(Camera& camera, GLfloat hAngle, GLfloat vAngle) {
    GLfloat vAngleInRad = MathUtils::degToRad(vAngle);
    GLfloat hAngleInRad = MathUtils::degToRad(hAngle);
    
    Vector newFacingDir(cos(vAngleInRad)*sin(hAngleInRad),
            sin(vAngleInRad),
            cos(vAngleInRad)*cos(hAngleInRad));
    newFacingDir.normalize();
    
    Vector right(sin(hAngleInRad - PI/2.0),
            0.0,
            cos(hAngleInRad - PI/2.0));
    right.normalize();
    
    Vector newUp = right.crossProduct(newFacingDir);
    newUp.normalize();
    
    camera.setFacingDirection(newFacingDir);
    camera.setUp(newUp);
}

void movementTracker(int value) {
    
    Vector upDir = eyeCamera.getUp();
    Vector facingDir = eyeCamera.getFacingDirection();
    Point oldPosition(eyeCamera.getPosition());
    Point newPosition(oldPosition);
    Vector forwardVector = facingDir;
    forwardVector.setY(0.0);
    forwardVector.normalize();
    Vector rightVector = facingDir.crossProduct(upDir).getNormalized();
    
    if (keyState[moveForwardKey]) {
        if (keyState[moveLeftKey]) {
            newPosition.set(oldPosition + (forwardVector - rightVector).getNormalized()*movingSpeed);
        }else if (keyState[moveRightKey]) {
            newPosition.set(oldPosition + (forwardVector + rightVector).getNormalized()*movingSpeed);
        }else {
            newPosition.set(oldPosition + forwardVector.getNormalized()*movingSpeed);
        }
    }else if (keyState[moveBackKey]) {
        if (keyState[moveLeftKey]) {
            newPosition.set(oldPosition + (forwardVector*(-1) - rightVector).getNormalized()*movingSpeed);
        }else if (keyState[moveRightKey]) {
            newPosition.set(oldPosition + (forwardVector*(-1) + rightVector).getNormalized()*movingSpeed);
        }else {
            newPosition.set(oldPosition + (forwardVector*(-1)).getNormalized()*movingSpeed);
        }
    }else if (keyState[moveLeftKey]) {
         newPosition.set(oldPosition - rightVector.getNormalized()*movingSpeed);
    }else if (keyState[moveRightKey]) {
         newPosition.set(oldPosition + rightVector.getNormalized()*movingSpeed);
    }
    
    
    eyeCamera.setPosition(newPosition);
    
    if (keyState[moveLightUpKey]) {
        lights[selectedLight].setY(lights[selectedLight].getY() + LIGHT_MOVING_SPEED);
    }else if (keyState[moveLightDownKey]) {
        lights[selectedLight].setY(lights[selectedLight].getY() - LIGHT_MOVING_SPEED);
    }else if (keyState[moveLightLeftKey]) {
        lights[selectedLight].setX(lights[selectedLight].getX() - LIGHT_MOVING_SPEED);
        lights[selectedLight].setZ(lights[selectedLight].getZ() + LIGHT_MOVING_SPEED);
    }else if (keyState[moveLightRightKey]) {
        lights[selectedLight].setX(lights[selectedLight].getX() + LIGHT_MOVING_SPEED);
        lights[selectedLight].setZ(lights[selectedLight].getZ() - LIGHT_MOVING_SPEED);
    }
    
    glutTimerFunc(30, movementTracker, 0);
    glutPostRedisplay();
}


void onKeyUp(unsigned char key, int x, int y) {  
    keyState[key] = false; // Set the state of the current key to not pressed  
}

void onKeybordEvent(unsigned char key, int x, int y ) {
    switch (key) {
        case 'e':
        case '\e':
            exit(1);
            break;
    }
    keyState[key] = true;
}
    
void onPassiveMotion(int x, int y) {
    GLsizei halfOfWidth = windowW/2, halfOfHeight = windowH/2;
    
    if (x != halfOfWidth || y != halfOfHeight) {
        glutWarpPointer(halfOfWidth, halfOfHeight);
        GLfloat hAngle = MOUSE_SENSITIVITY*(halfOfWidth - x);
        GLfloat vAngle = MOUSE_SENSITIVITY*(halfOfHeight - y);
        facingHAngle += hAngle;
        facingVAngle += vAngle;
        
        if (facingVAngle < -89.0) {
            facingVAngle = -89.0;
        }
        
        if (facingVAngle > 89.0) {
            facingVAngle = 89.0;
        }
        
        turnCamera(eyeCamera, facingHAngle, facingVAngle);
        glutPostRedisplay();
    }
}

void onMouseEvent(int button, int state, int x, int y) {
   
    
    if ((button == 3) || (button == 4)) {

        // Disregard redundant GLUT_UP events
        if (state == GLUT_UP) {
            return;
        }

        switch ( button ) {
            //scroll up
            case 3:
                movingSpeed += 0.05;
                break;
            //scroll down
            case 4:
                movingSpeed -= 0.05;
                break;
        }
    }
}

void drawTexturedPentagon() {
    int v;
    GLfloat ang;
    GLfloat vertexAngle = MathUtils::degToRad(360.0)/5.0;  
    GLfloat angleOffset = MathUtils::degToRad(54.0);

    //reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBegin (GL_POLYGON);
    GLfloat k1 = 6.0;
    GLfloat k2 = 3.5;
    for (v = 0; v < 5; v++)  {                  
        ang = v * vertexAngle - angleOffset;
        
        glTexCoord2d(-cos(ang)/k1*k2+0.5, -sin(ang)/k1*k2+0.5);
        glVertex2f(cos(ang), sin(ang));
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawSimplePentagon(const Point& pentagonColor) {
    glColor3f(pentagonColor.getX(), pentagonColor.getY(), pentagonColor.getZ());
    int v;
    GLfloat ang;
    GLfloat vertexAngle = MathUtils::degToRad(360.0)/5.0;  
    GLfloat angleOffset = MathUtils::degToRad(54.0);

    glBegin (GL_POLYGON);
    for (v = 0; v < 5; v++)  {                  
        ang = v * vertexAngle - angleOffset;
        glVertex2f(cos(ang), sin(ang));
    }
    glEnd();
}

void drawPentagon(const Point& pentagonColor, GLint type) {
    if (type == 0) {
        drawSimplePentagon(pentagonColor);
    }else if (type == TEXTURED_TYPE) {
        drawTexturedPentagon();
    }
}

Triangle t;

void initTriangle(GLfloat triangleSide, 
        GLfloat upperCenterOffsetUpwards) {
    
    GLfloat x        =  triangleSide/2.0;
    GLfloat aukstine =  sqrt(3.0)*x;

    t.set(-x   , -(aukstine - upperCenterOffsetUpwards) , 0.0f ,
           0.0f , upperCenterOffsetUpwards               , 0.0f ,
          +x   , -(aukstine - upperCenterOffsetUpwards)  , 0.0f);
}

void drawTriangles(const Point& triangleColor, GLfloat pentagonDistToConer,
        GLfloat triangleSide, GLfloat upperCenterOffsetUpwards, 
        GLfloat downAngle, int type) {
    glColor3f(triangleColor.getX(), triangleColor.getY(), triangleColor.getZ());
    initTriangle(triangleSide, upperCenterOffsetUpwards);

    GLfloat rotationLeftAngle = -2.0*36.0;
    
    glPushMatrix();
    for(int i = 0; i < 5; i++) {
        GLfloat rotAngle = i == 0 ? rotationLeftAngle/2.0 : rotationLeftAngle;
        glRotatef(rotAngle, 0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0, -pentagonDistToConer, 0.0);
        glRotatef(downAngle, 1.0, 0.0, 0.0);
        if (type == CYLINDRIC_TYPE) {
            t.drawCylindric(sphereColor, cylinderColor, qobj,
                       cylinderRadius, cylinderSlices, cylinderSlacks,
                       sphereRadius, sphereSlices, sphereSlacks);
        }else  if (type == TEXTURED_TYPE) {
            t.drawTextured();
        }else {
            t.draw();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawCylindricSquare(const Point& cylinderColor, GLfloat squareSide, 
        GLUquadric* qobj, GLfloat radius,
        GLsizei slices, GLsizei stacks) {

    GLfloat x = squareSide/2.0;
    glColor3f(cylinderColor.getX(), cylinderColor.getY(), cylinderColor.getZ());
    Point p1(-x, -squareSide, zOffset);
    Point p2(-x, 0.0f, zOffset);
    Point p3(x, 0.0f, zOffset);
    Point p4(x, -squareSide, zOffset);
    Utils::drawCylinder(p1, p2, qobj, radius, Point::distance(p1, p2), slices, 
            stacks);
    Utils::drawCylinder(p2, p3, qobj, radius, Point::distance(p2, p3), slices, 
            stacks);
    Utils::drawCylinder(p3, p4, qobj, radius, Point::distance(p3, p4), slices, 
            stacks);
    Utils::drawCylinder(p4, p1, qobj, radius, Point::distance(p4, p1), slices, 
            stacks);
}

void drawSimpleSquare(const Point& squareColor, GLfloat squareSide) {
    GLfloat x = squareSide/2.0;
    glColor3f(squareColor.getX(), squareColor.getY(), squareColor.getZ());
    glBegin(GL_POLYGON);
    //clock-wise
    glVertex3f(-x, -squareSide, zOffset);
    glVertex3f(-x, 0.0f, zOffset);
    glVertex3f(x, 0.0f, zOffset);
    glVertex3f(x, -squareSide, zOffset);
    glEnd();
}

void drawTexturedSquare(GLfloat squareSide) {
    //reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    GLfloat x = squareSide/2.0;
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    //clock-wise
//    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2d(1.0, 1.0); glVertex3f(-x, -squareSide, zOffset);
    glTexCoord2d(1.0, 0); glVertex3f(-x, 0.0f, zOffset);
    glTexCoord2d(0, 0); glVertex3f(x, 0.0f, zOffset);
    glTexCoord2d(0, 1.0); glVertex3f(x, -squareSide, zOffset);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawSquare(const Point& squareColor, GLfloat squareSide, 
        int type) {
    if (type == CYLINDRIC_TYPE) {
        drawCylindricSquare(cylinderColor, squareSide, qobj, cylinderRadius,
                cylinderSlices, cylinderSlacks);
    }else if (type == TEXTURED_TYPE) {
        drawTexturedSquare(squareSide);
    }else {       
        drawSimpleSquare(squareColor, squareSide);
    }
}

void drawSquares(const Point& squaresColor, GLfloat pentagonDistToWall, 
        GLfloat squareSide, GLfloat downAngle, int type) {

    GLfloat dist = pentagonDistToWall;
    glPushMatrix();
    glTranslatef(0.0, -pentagonDistToWall, 0.0);
    glRotatef(downAngle, 1.0, 0.0, 0.0);
    drawSquare(squaresColor, squareSide, type);
    glPopMatrix();
    
    GLfloat rotAngle = 360.0 - 72.0;

    glPushMatrix();
    for(int i = 0; i < 4; i++) {
        glRotatef(rotAngle, 0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0, -pentagonDistToWall, 0.0);
        glRotatef(downAngle, 1.0, 0.0, 0.0);
        drawSquare(squaresColor, squareSide, type);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawFace(const Point& triangleColor, const Point& squareColor, 
        const Point& pentagonColor, int type) {

    GLfloat pentagonDistToCorner = 1.0;
    GLfloat pentagonDistToWall = sin(MathUtils::degToRad(54.0));
    drawPentagon(pentagonColor, type);

    GLfloat cof = 31.0/26.360390158f;
    GLfloat squaresDownAngle = 31.770390158f;
    GLfloat trianglesDownAngle = squaresDownAngle*cof;

    GLfloat squareSide = 2.0*sin(MathUtils::degToRad(36.0));
    drawSquares(squareColor, pentagonDistToWall, squareSide, 
            squaresDownAngle, type);

    GLfloat triangleSide = squareSide;
    GLfloat upperCenterOffsetUpwards = 0.0f;
    drawTriangles(triangleColor, pentagonDistToCorner, triangleSide, 
            upperCenterOffsetUpwards, trianglesDownAngle, type);
}

void drawHalf(const Point& triangleColor, const Point& squareColor, 
        const Point& pentagonColor, int type) {

    glPushMatrix();
    drawFace(triangleColor, squareColor, pentagonColor, type);
    glPopMatrix();

    GLfloat squaresDownAngle = 31.770390158f;
    GLfloat squareSide = 2.0*sin(MathUtils::degToRad(36.0));
    GLfloat heightDiff = squareSide*sin(MathUtils::degToRad(squaresDownAngle));
    GLfloat dist = 2.41;

    GLfloat rotAngle = 36.0f;
    GLfloat downAngle = 36.0f;
    GLfloat rotMatrix[16];

    GLfloat pentagonDistToCorner = 1.0;
    GLfloat pentagonDistToWall = sin(MathUtils::degToRad(54.0));

    for(int i = 0; i < 5; i++) {

        GLfloat curAngle = rotAngle + 72*i;
        glPushMatrix();

        glRotatef(curAngle , 0.0 , 0.0 , 1.0);
        glTranslatef(0, pentagonDistToWall, 0.0f);
        //rotate down
        glRotatef(-2*squaresDownAngle, 1.0 , 0.0 , 0.0);
        glTranslatef(0, pentagonDistToWall, 0.0f);
        glTranslatef(0.0, 1.0, heightDiff);
        drawFace(triangleColor, squareColor, pentagonColor, type);
        
        glPopMatrix();
    }
}

void drawBall(const Point& center, const Point& triangleColor, const Point& squareColor, 
        const Point& pentagonColor, int type) {
    glPushMatrix();

    //move sphere to center
    GLfloat sphereRadius = 4.86;
    glTranslatef(center.getX(), center.getY(), center.getZ() + sphereRadius/2.0);

    glPushMatrix();
    drawHalf(triangleColor, squareColor, pentagonColor, type);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, sphereRadius);
    drawHalf(triangleColor, squareColor, pentagonColor, type);
    glPopMatrix();

    glPopMatrix();
}


void initQuadric() {
  qobj = gluNewQuadric();
  gluQuadricNormals(qobj, GLU_SMOOTH);
}

void drawCylinder() {

  gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 5);
}


void shadowTransform(const GLfloat plane[], const GLfloat light[]) {
    GLfloat m[16];
    const GLfloat k = plane[0] * light[0] + plane[1] * light[1]
    + plane[2] * light[2] + plane[3] * light[3];
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            m[4*i+j] = -plane[i] * light[j];
        }
        m[5*i] += k; // add k to diagonal entries
    }
    glMultMatrixf(m);
}

void drawShadow(const Point& ballCenter, const GLfloat plane[], const GLfloat light[]) {
    Point colors[] = {cylinderColor, sphereColor};
    cylinderColor.set(black);
    sphereColor.set(black);
    glPushMatrix();
    shadowTransform(plane, light);
    drawBall(ballCenter, Point(black), Point(black), Point(black), CYLINDRIC_TYPE);
    glPopMatrix();
    cylinderColor.set(colors[0]);
    sphereColor.set(colors[1]);
}

void drawShadows(const Point& ballCenter, const Point& light) {
    //if we want to have directional light use 0.0 instead of 1.0
    GLfloat light0[] = {light.getX(), light.getY(), light.getZ(), 1.0};
    
    float yzPlane[] = {1.0, 0.0, 0.0, 0.0};
    drawShadow(ballCenter, yzPlane, light0);
    
    float yxPlane[] = {0.0, 0.0, 1.0, 0.0};
    drawShadow(ballCenter, yxPlane, light0);
}


void display() {

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Point cameraPosition = eyeCamera.getPosition();
    Vector facingDirection = eyeCamera.getFacingDirection();
    Vector cameraUp = eyeCamera.getUp();

    gluLookAt(cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ(),
            (cameraPosition.getX() + facingDirection.getX()),
            (cameraPosition.getY() + facingDirection.getY()),
            (cameraPosition.getZ() + facingDirection.getZ()),
            0.0, 1.0, 0.0);

    glPushMatrix();
    GLfloat slabWidth = 2.0;
    GLfloat slabHeight = 2.0;
    GLsizei slabsInWidth = 15;
    GLsizei slawbsInHeight = 15; 
    drawFloor(0.0, 0.0, 0.0, slabsInWidth, slawbsInHeight, slabWidth, slabHeight);
    drawWalls(-0.01, 0.0, -0.01, slabWidth*slabsInWidth, slabHeight*slawbsInHeight);
    drawXYZAxis(7.0);
    Utils::drawCube(lights[0], 0.5);
    drawBall(cylindricBallCenter, Point(peachpuff), Point(green), Point(red), CYLINDRIC_TYPE);
    drawBall(texturedBallCenter, Point(peachpuff), Point(green), Point(red), TEXTURED_TYPE);
    glPopMatrix();
    
    Point light0(lights[0]);
    GLfloat lightpos[] = {light0.getX(), light0.getY(), light0.getZ(), 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    drawShadows(cylindricBallCenter, lights[0]);
    
    
//    glPushMatrix();
//    glTranslatef(5.0, 3, 5.0);
////    drawTexturedSquare(2.0);
////    drawTexturedPentagon();
//    drawFace(Point(black), Point(black), Point(black), TEXTURED_TYPE);
//    glPopMatrix();
    
//    drawTexturedSquare(1.0);
    
    glutSwapBuffers();  
}

void reshape(GLsizei w, GLsizei h) {  
    windowW = w;
    windowH = h;
    glViewport (0, 0, w, h);
    aspect = (GLfloat)w / (GLfloat)h;
    glutPostRedisplay ();
}

void initGL() {
    glMatrixMode (GL_PROJECTION);
    gluPerspective (65, aspect, 1, 100);
    glClearColor (black, 0);
    glEnable (GL_COLOR_MATERIAL);
    glEnable (GL_DEPTH_TEST);
    glEnable ( GL_LIGHTING );
    glEnable(GL_LIGHT0);
}

void loadTexture(GLuint& textureId, const char* fullPath) {
    glActiveTexture(GL_TEXTURE0);
    textureId = SOIL_load_OGL_texture(fullPath, SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y);
    if(textureId == 0) {
        cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << fullPath << ")" << endl;
        return;
    }
    
    int imgWidth, imgHeight;
    unsigned char* img = SOIL_load_image(fullPath, &imgWidth, &imgHeight, NULL, 0);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0,  GL_RGB, GL_UNSIGNED_BYTE, img);
    
//    SOIL_free_image_data(img);
    
    //Unbind texture
//    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR) {
        cerr << "Error loading texture: " << gluErrorString( error ) << endl;
    }
}

void initVars() {
    //initialize Quadric for cylinders and spheres
    initQuadric();
    //set initial position of camera
    eyeCamera.setPosition(Point(15.0, 1.3, 15.0));
    //set initial facing of camera
    facingHAngle = 190.0;
    turnCamera(eyeCamera, facingHAngle, facingVAngle);
    //set initial position of lights
    lights[0].set(15.0, 10.0, 15.0);
    //initialize texture
    loadTexture(textureId, "/home/zilva/vu/kompiuterine-grafika/3/t8.jpg");
}

int main( int argc, char** argv ) {

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    // Enable lighting
    glutInitWindowSize(750, 650);   
    glutInitWindowPosition(600, 50); 
    glutCreateWindow("Rhombicosidodecahedron");          
    
    glutPassiveMotionFunc(onPassiveMotion);
    glutMouseFunc(onMouseEvent);
    glutKeyboardUpFunc(onKeyUp); 
    glutKeyboardFunc(onKeybordEvent);
    glutDisplayFunc(display);       
    glutReshapeFunc(reshape);
    glutSetCursor(GLUT_CURSOR_NONE); 
    initGL();         
    initVars();
    movementTracker(0);
    glutMainLoop(); 
    gluDeleteQuadric(qobj);    
    return 0;
}