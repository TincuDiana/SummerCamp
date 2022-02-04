#include "Camera.hpp"
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define SEEK_END 2
using namespace std;
#include <fstream>


namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->angleX = 0;
        this->angleY = 0;
        //TODO - Update the rest of camera parameters

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    //verificam mereu sa ne aflam in domeniul planului
    bool isValid(glm::vec3 *dir)
    {
          if ((*dir).x < -300 || (*dir).x > 300  || (*dir).z > 400 || (*dir).z < -400  || (*dir).y < -1 || (*dir).y > 120)
           {
              if ((*dir).x < -300)
              {
                  (*dir).x += 1;
              }
              if ((*dir).x > 300)
              {
                  (*dir).x -= 1;
              }
              if ((*dir).z < -400)
              {
                  (*dir).z += 1;
              }
              if ((*dir).z > 400)
              {
                  (*dir).z -= 1;
              }
              if ((*dir).y < -1)
              {
                  (*dir).y += 2;
              }
              if ((*dir).y > 120)
              {
                  (*dir).y -= 2;
              }
              return false;
           }
          return true;
    }
    
    void Camera::presentation(float speed, glm::vec3 dir)
    {
        std::cout << "\n1";
        this->cameraFrontDirection = dir;
        this->cameraPosition += this->cameraFrontDirection * (speed*2);
        this->cameraTarget += this->cameraFrontDirection * (speed*2);
    }

    void Camera::move(MOVE_DIRECTION direction, float speed) {
        speed *= 50;
        int x = this->cameraPosition.x;
        int z = this->cameraPosition.z ;
        std::cout << cameraPosition.x << "f, " << cameraPosition.y << "f, " << cameraPosition.z<<"f \n";
        bool ver = true;
        //presentation();

        switch (direction)
        {
        case MOVE_FORWARD:        
            if (isValid(&this->cameraPosition))
            {
                this->cameraPosition += this->cameraFrontDirection * speed;
                this->cameraTarget += this->cameraFrontDirection * speed;
            }     
          
            break;
        case MOVE_BACKWARD:
            if (isValid(&this->cameraPosition))
            {
                this->cameraPosition -= this->cameraFrontDirection * speed;
                this->cameraTarget -= this->cameraFrontDirection * speed;
            }      
            break;
        case MOVE_RIGHT:
            if (isValid(&this->cameraPosition))
            {
                this->cameraPosition += this->cameraRightDirection * speed;
                this->cameraTarget += this->cameraRightDirection * speed;
            }
            break;
        case MOVE_LEFT:
            if (isValid(&this->cameraPosition))
            {
                this->cameraPosition -= this->cameraRightDirection * speed;
                this->cameraTarget -= this->cameraRightDirection * speed;
            }
            break;
        }
    
    }

    void Camera::rotate(float pitch, float yaw) {
        this->angleY += yaw;
        this->angleX += pitch;
        cameraTarget = glm::vec3(cameraPosition.x+sin(angleY), cameraPosition.y + sin(angleX), cameraPosition.z-cos(angleY));
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::cross(this->cameraFrontDirection, this->cameraUpDirection);
    }

}