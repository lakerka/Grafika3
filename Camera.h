/* 
 * File:   Camera.h
 * Author: zilva
 *
 * Created on January 10, 2015, 4:35 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "Point.h"
#include "Vector.h"

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();
    
    void setPosition(const Point& newPosition);
    void setFacingDirection(const Vector& newFacingDirection);
    void setUp(const Vector& newUp);
    
    Point getPosition() const;
    Vector getFacingDirection() const;
    Vector getUp() const;
    
private:
    Point position;
    Vector facingDirection;
    Vector up;
};

#endif	/* CAMERA_H */

