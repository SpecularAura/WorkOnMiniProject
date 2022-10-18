#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
    //default values for any camera class
    static constexpr float YAW{-90.0f};
    static constexpr float PITCH{0.0f};
    static constexpr float SPEED{2.5f};
    static constexpr float SENSITIVITY{0.1f};
    static constexpr float ZOOM{45.0f};

    glm::vec3 Position, Front, Up, Right, WorldUp;

    float Yaw, Pitch, MovementSpeed, MouseSensitivity, Zoom;

    void UpdateCamera();

    public:
    // Initialize the Camera with the default values given the position of the camera & worlup vector
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 wup = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH); 
    
    // Constructor overload for alternate Inititalization
    Camera(float posX, float posY, float posZ,
           float wupX, float wupY, float wupZ);

    glm::mat4 ViewMatrix();
    static glm::mat4 lookat(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    void MoveCamera(CameraMovement direction, float deltatime);
    void RotateCamera(float xoffset, float yoffset, bool constrainPitch = true);
    void ZoomCamera(float yoffset);
    float GetZoom();
};



#endif
