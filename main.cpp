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
#include "Utils.h"
#include "Point.h"
#include "Triangle.h"
#include "Colors.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Camera.h"
#include <GL/glut.h>
#include <cmath>	// OpenGL Graphics Utility Library

using namespace std;

enum Direction { LEFT, RIGHT, UP, DOWN};
char MOVE_LEFT = 'a';
char MOVE_RIGHT = 'd';
char MOVE_FORWARD = 'w';
char MOVE_BACK = 's';

int keyState[500];

GLUquadric* qobj;
Camera eyeCamera;
GLsizei windowW, windowH;
GLfloat facingHAngle, facingVAngle; 
GLfloat movingSpeed = MOVING_SPEED;

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
    GLfloat vAngleInRad = Utils::degToRad(vAngle);
    GLfloat hAngleInRad = Utils::degToRad(hAngle);
    
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
    
//    cout << keyState[MOVE_FORWARD] << " " << keyState[MOVE_LEFT] << " " << keyState[MOVE_RIGHT] << endl;
    if (keyState[MOVE_FORWARD]) {
        if (keyState[MOVE_LEFT]) {
            newPosition.set(oldPosition + (forwardVector - rightVector).getNormalized()*movingSpeed);
        }else if (keyState[MOVE_RIGHT]) {
            newPosition.set(oldPosition + (forwardVector + rightVector).getNormalized()*movingSpeed);
        }else {
            newPosition.set(oldPosition + forwardVector.getNormalized()*movingSpeed);
        }
    }else if (keyState[MOVE_BACK]) {
        if (keyState[MOVE_LEFT]) {
            newPosition.set(oldPosition + (forwardVector*(-1) - rightVector).getNormalized()*movingSpeed);
        }else if (keyState[MOVE_RIGHT]) {
            newPosition.set(oldPosition + (forwardVector*(-1) + rightVector).getNormalized()*movingSpeed);
        }else {
            newPosition.set(oldPosition + (forwardVector*(-1)).getNormalized()*movingSpeed);
        }
    }
    eyeCamera.setPosition(newPosition);
    
    glutTimerFunc(20, movementTracker, 0);
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

void drawPentagon() {

    glColor3f(darkPurple);
    int v;
    GLfloat ang;
    GLfloat vertexAngle = Utils::degToRad(360.0)/5.0;  
    GLfloat angleOffset = Utils::degToRad(54.0);

    glBegin (GL_POLYGON);
    for (v = 0; v < 5; v++)  {                  
        ang = v * vertexAngle - angleOffset;
        glVertex2f(cos(ang), sin(ang));
    }
    glEnd();
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

void drawTriangles(GLfloat pentagonDistToConer,
            GLfloat triangleSide, 
            GLfloat upperCenterOffsetUpwards,
            GLfloat downAngle) {
    glColor3f(black);
    initTriangle(triangleSide, upperCenterOffsetUpwards);

    GLfloat rotationLeftAngle = -2.0*36.0;
    
    glPushMatrix();
    for(int i = 0; i < 5; i++) {
        GLfloat rotAngle = i == 0 ? rotationLeftAngle/2.0 : rotationLeftAngle;
        glRotatef(rotAngle, 0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0, -pentagonDistToConer, 0.0);
        glRotatef(downAngle, 1.0, 0.0, 0.0);
        t.draw();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawSquare(GLfloat squareSide) {

    GLfloat x = squareSide/2.0;
    glColor3f(peachpuff);
    glBegin(GL_POLYGON);
    //clock-wise
    glVertex3f(-x, -squareSide, zOffset);
    glVertex3f(-x, 0.0f, zOffset);
    glVertex3f(x, 0.0f, zOffset);
    glVertex3f(x, -squareSide, zOffset);
    glEnd();
}

void drawSquares(GLfloat pentagonDistToWall, GLfloat squareSide, GLfloat downAngle) {

    GLfloat dist = pentagonDistToWall;
    glPushMatrix();
    glTranslatef(0.0, -pentagonDistToWall, 0.0);
    glRotatef(downAngle, 1.0, 0.0, 0.0);
    drawSquare(squareSide);
    glPopMatrix();
    
    GLfloat rotAngle = 360.0 - 72.0;

    glPushMatrix();
    for(int i = 0; i < 4; i++) {

        glRotatef(rotAngle, 0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(0.0, -pentagonDistToWall, 0.0);
        glRotatef(downAngle, 1.0, 0.0, 0.0);
        drawSquare(squareSide);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawFace() {

    GLfloat pentagonDistToCorner = 1.0;
    GLfloat pentagonDistToWall = sin(Utils::degToRad(54.0));
    drawPentagon();

    GLfloat cof = 31.0/26.360390158f;
    GLfloat squaresDownAngle = 31.770390158f;
    GLfloat trianglesDownAngle = squaresDownAngle*cof;

    GLfloat squareSide = 2.0*sin(Utils::degToRad(36.0));
    drawSquares(pentagonDistToWall,  squareSide, squaresDownAngle);

    GLfloat triangleSide = squareSide;
    GLfloat upperCenterOffsetUpwards = 0.0f;
    drawTriangles(pentagonDistToCorner, triangleSide, upperCenterOffsetUpwards, trianglesDownAngle);
}

void drawHalf() {

    glPushMatrix();
    drawFace();
    glPopMatrix();

    GLfloat squaresDownAngle = 31.770390158f;
    GLfloat squareSide = 2.0*sin(Utils::degToRad(36.0));
    GLfloat heightDiff = squareSide*sin(Utils::degToRad(squaresDownAngle));
    GLfloat dist = 2.41;

    GLfloat rotAngle = 36.0f;
    GLfloat downAngle = 36.0f;
    GLfloat rotMatrix[16];

    GLfloat pentagonDistToCorner = 1.0;
    GLfloat pentagonDistToWall = sin(Utils::degToRad(54.0));

    for(int i = 0; i < 5; i++) {

        GLfloat curAngle = rotAngle + 72*i;
        glPushMatrix();

        glRotatef(curAngle , 0.0 , 0.0 , 1.0);
        glTranslatef(0, pentagonDistToWall, 0.0f);
        //rotate down
        glRotatef(-2*squaresDownAngle, 1.0 , 0.0 , 0.0);
        glTranslatef(0, pentagonDistToWall, 0.0f);
        glTranslatef(0.0, 1.0, heightDiff);
        drawFace();
        
        glPopMatrix();
    }
}

void drawBall() {
    glPushMatrix();

    glTranslatef(10.0, 3.0, 10.0);

    //move sphere to center
    GLfloat sphereRadius = 4.86;
    glTranslatef(0.0, 0.0, sphereRadius/2);

    glPushMatrix();
    drawHalf();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, sphereRadius);
    drawHalf();
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

void display() {

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    glClearColor (white,  0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Point cameraPosition = eyeCamera.getPosition();
    Vector facingDirection = eyeCamera.getFacingDirection();
    Vector cameraUp = eyeCamera.getUp();

    gluLookAt(cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ(),
            (cameraPosition.getX() + facingDirection.getX()),
            (cameraPosition.getY() + facingDirection.getY()),
            (cameraPosition.getZ() + facingDirection.getZ()),
            0.0, 1.0, 0.0);
//    drawDirectionAndUpAxis(eyeCamera, 5.0);


    GLfloat slabWidth = 2.0;
    GLfloat slabHeight = 2.0;
    GLsizei slabsInWidth = 15;
    GLsizei slawbsInHeight = 15; 
    drawFloor(0.0, 0.0, 0.0, slabsInWidth, slawbsInHeight, slabWidth, slabHeight);
//    drawWalls(0.0, 0.0, 0.0, slabWidth*slabsInWidth, slabHeight*slawbsInHeight);
//    drawBall();
    drawXYZAxis(10.0);
    drawCylinder();
    
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
    glClearColor (1, 1, 1, 0);
    glEnable (GL_COLOR_MATERIAL);
    glEnable (GL_DEPTH_TEST);
    initQuadric();
//    glEnable(GL_LIGHTING);
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
    //set initial position of camera
    eyeCamera.setPosition(Point(15.0, 1.3, 15.0));
    movementTracker(0);
    glutMainLoop(); 
    gluDeleteQuadric(qobj);    
    return 0;
}