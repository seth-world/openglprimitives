#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>

#include <zvertice.h>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT ,
    UP,
    DOWN,

    LIGHTFORWARD,
    LIGHTBACKWARD,
    LIGHTLEFT,
    LIGHTRIGHT,
    LIGHTHIGH,
    LIGHTLOW,
    LIGHTORIGIN,

    TOGGLE_GAMMA,

    TOGGLE_POLYGONLINES,
    TOGGLE_NORMALVECTORS,
    TOGGLE_MOUSE,

    RESET_POSITION


};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 CameraPosition;
    glm::vec3 DefaultCameraPosition;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    const float DefaultKeyAccelerator=0.01f;
    float KeyAccelerator=DefaultKeyAccelerator;

    bool        redrawRequest = true;

    glm::vec3   LightPosition=glm::vec3 (1.0f, 1.0f, 1.0f) ;
    glm::vec3   DefaultLightPosition=LightPosition ;

    glm::vec3   RotationAxis=glm::vec3(0.0f);
    float       RotationAngle=0.0f;

    bool        usePolygonLines=false;
    inline void setPolygonLines(bool pOnOff)
            {
            usePolygonLines=pOnOff;
            scheduleRedraw();
            }
    inline void togglePolygonLines()
            {
            setPolygonLines(!usePolygonLines);
            }
    bool        useNormalVectors=false;
    inline void setNormalVectors(bool pOnOff)
            {
            useNormalVectors=pOnOff;
            scheduleRedraw();
            }
    inline void toggleNormalVectors()
            {
            setNormalVectors(!useNormalVectors);
            }
    bool        useGammaCorrection=false;
    inline void setGammaCorrection(bool pOnOff)  /*generates glEnable(GL_FRAMEBUFFER_SRGB); or glDisable(GL_FRAMEBUFFER_SRGB); */
            {
            useGammaCorrection=pOnOff;
            scheduleRedraw();
            }
    inline void toggleGammaCorrection()  /*generates glEnable(GL_FRAMEBUFFER_SRGB); or glDisable(GL_FRAMEBUFFER_SRGB); */
            {
            useGammaCorrection=!useGammaCorrection;
            scheduleRedraw();
            }
    inline void toggleMouse()  /*generates glEnable(GL_FRAMEBUFFER_SRGB); or glDisable(GL_FRAMEBUFFER_SRGB); */
            {
            FollowMouse=!FollowMouse;
            }
    inline void resetPositions()
    {
       LightPosition=DefaultLightPosition;
       CameraPosition=DefaultCameraPosition;
       updateCameraVectors();
       scheduleRedraw();
    }

    float       LightMove=0.1f;

    bool FollowMouse=false;
    bool ShiftOn=false;
    bool Rotation=false;

    // Constructor with vectors
    Camera(glm::vec3 pCameraPosition ,
           glm::vec3 pLightPosition ,
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        CameraPosition = pCameraPosition;
        DefaultCameraPosition = pCameraPosition;
        LightPosition=pLightPosition;
        DefaultLightPosition=pLightPosition;

        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Right =glm::vec3(1,0,0);
        updateCameraVectors();
        scheduleRedraw();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        CameraPosition = glm::vec3(posX, posY, posZ);
        DefaultCameraPosition = CameraPosition;
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
        scheduleRedraw();
    }


    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(CameraPosition, CameraPosition + Front, Up);
    }

    const char* decodeCameraMovement(Camera_Movement direction)
    {
        switch (direction)
        {
        case FORWARD:
            return "FORWARD";
        case BACKWARD:
            return "BACKWARD";
        case LEFT:
            return "LEFT";
        case RIGHT:
            return "RIGHT";
        }
    }

    inline void scheduleRedraw()
    {
        redrawRequest=true;
    }
    inline void cancelRedraw()
    {
        redrawRequest=false;
    }
    bool redrawRequested() {return  redrawRequest;}


    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float delta)
    {
        KeyAccelerator += delta;
        if (KeyAccelerator>0.5)
                    KeyAccelerator=0.5;
//        float velocity = MovementSpeed * deltaTime==0.0f?0.01:deltaTime;
        float velocity = MovementSpeed * KeyAccelerator;
         switch (direction)
        {
/* Camera movements */
        case FORWARD:
             ProcessMouseScroll(0.1f);
//            CameraPosition += Front * velocity;
            return;
        case BACKWARD:
             ProcessMouseScroll(-0.1f);
//            CameraPosition -= Front * velocity;
            return;
        case LEFT:
            CameraPosition -= Right * velocity;
            scheduleRedraw();
            return;
        case RIGHT:

            CameraPosition += Right * velocity;
            scheduleRedraw();
            return;
         case UP:
//             CameraPosition += Front * velocity;
              CameraPosition += Up * velocity;
             scheduleRedraw();
             return;
         case DOWN:
//             CameraPosition -= Front * velocity;
              CameraPosition -= Up * velocity;
             scheduleRedraw();

             return;
/* Light movements */
        case LIGHTFORWARD:
            LightPosition.z += LightMove * velocity;
            return;
        case LIGHTBACKWARD:
            LightPosition.z -= LightMove * velocity;
            return;

        case LIGHTLEFT:
            LightPosition.x -= LightMove * velocity;
            return;
        case LIGHTRIGHT:
            LightPosition.x += LightMove * velocity;
            return;
        case LIGHTLOW:
            LightPosition.y-= LightMove * velocity;
            return;
        case LIGHTHIGH:
            LightPosition.y += LightMove * velocity;
            return;


        case TOGGLE_POLYGONLINES:
            togglePolygonLines() ;
            return;

         case TOGGLE_GAMMA:
             toggleGammaCorrection();
             return;
         case TOGGLE_NORMALVECTORS:
             toggleNormalVectors();
             return;
         case TOGGLE_MOUSE:
             toggleMouse();
             return;
//         case RESET_POSITION:
//             resetPosition();
             return;

        }// swich

        scheduleRedraw();
        return;
    }

    void toggleMouseTracking()
    {
        FollowMouse=!FollowMouse;
    }

    void setLightPosition(glm::vec3 pPosition)
    {
        LightPosition=pPosition;
        scheduleRedraw();
    }

    void setShiftOn()
    {
        ShiftOn=true;
        scheduleRedraw();
    }
    void setShiftOff()
    {
        ShiftOn=false;
        scheduleRedraw();
    }

    void resetKeyAccelerator()
    {
        KeyAccelerator=DefaultKeyAccelerator;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        if (Rotation)
        {
//            RotationAxis=RotationAxis+glm::vec3(xoffset>0.0f?1.0f:0.0f,yoffset>0.0f?1.0f:0.0f,0.0f);
//            RotationAxis=RotationAxis+camera.rotateModel(-0.05,glm::vec3(0.0f,1.0f,0.0f));;
//            RotationAngle += 0.5f;
            RotationAngle = 0.5f;
            rotateModel(0.5f,glm::vec3(xoffset*MouseSensitivity/2.0f,yoffset*MouseSensitivity/2.0f,0.0f));
            scheduleRedraw();
        }
        if (!FollowMouse)
                return;
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
        scheduleRedraw();
    }

    void setLightPosition(glm::vec3& pPosition)
    {
        LightPosition=pPosition;
        scheduleRedraw();
    }

    void ProcessMouseForLight(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        if (!FollowMouse)
                return;
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        LightPosition.x += xoffset;
        LightPosition.y += yoffset;


        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
        scheduleRedraw();
    }



    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
     /*   printf ("Mouse scroll  : Zoom <%g> yoffset <%g> ",
                Zoom,
                yoffset
                );*/
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;

        scheduleRedraw();
        return;
    }

    void ResetPositions()
    {
        LightPosition=DefaultLightPosition;
        CameraPosition=DefaultCameraPosition;
        scheduleRedraw();
    }

public:

    void setModelIdentity() {model=glm::mat4(1.0f);}
    glm::mat4 getModel() {return model;}
    glm::mat4 rotateModel(float pRotationAngle, glm::vec3 pRotationAxis)
    {
        model=glm::rotate(model,pRotationAngle,pRotationAxis);
        return model;
    }

protected:

    glm::mat4 model=glm::mat4(1.0f); /* model stores the rotation state for all objects in world before translation */
        /* it is reset to identity matrix to reset object's rotation */

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }








};
#endif
