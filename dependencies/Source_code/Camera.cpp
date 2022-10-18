#include "Camera.hpp"

Camera::Camera(glm::vec3 position ,
               glm::vec3 wup ,
               float yaw, float pitch ) :
               Position{position}, WorldUp{wup}, Front{0.0f, 0.0f, -1.0f}, Yaw{yaw}, Pitch{pitch},
               MovementSpeed{SPEED}, MouseSensitivity{SENSITIVITY}, Zoom{ZOOM}
{
    UpdateCamera();
}

Camera::Camera(float posX, float posY, float posZ,
           float wupX, float wupY, float wupZ) : Camera(glm::vec3(posX,posY,posZ), 
                                                        glm::vec3(wupX, wupY,wupZ))
{
}
void Camera::UpdateCamera()
{
    glm::vec3 front;
    front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    front.y = glm::sin(glm::radians(Pitch));
    front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    Front   = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::MoveCamera(CameraMovement direction, float deltatime)
{
    float velocity = MovementSpeed * deltatime;
    if(direction == CameraMovement::FORWARD)
        Position += velocity * Front;
    if(direction == CameraMovement::BACKWARD)
        Position -= velocity * Front;
    if(direction == CameraMovement::LEFT)
        Position -= velocity * Right;
    if(direction == CameraMovement::RIGHT)
        Position += velocity * Right;
}

void Camera::RotateCamera(float xoffset, float yoffset, bool constrainPitch)
{
    Yaw   += xoffset * MouseSensitivity;
    Pitch += yoffset * MouseSensitivity;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        else if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }
    }
    UpdateCamera();
}

void Camera::ZoomCamera(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::mat4 Camera::ViewMatrix()
{
    return glm::lookAt(Position, Position + Front, WorldUp);
}

glm::mat4 Camera::lookat(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    glm::vec3 Direction{eye - center};
    glm::vec3 Right{glm::normalize(glm::cross(up, Direction))};
    glm::vec3 Camup{glm::normalize(glm::cross(Direction, Right))};

    glm::mat4 lookat{1.0f};
    lookat[0][0] = Right.x;
    lookat[0][1] = Camup.x;
    lookat[0][2] = Direction.x; 
    lookat[1][0] = Right.y;
    lookat[1][1] = Camup.y;
    lookat[1][2] = Direction.y;
    lookat[2][0] = Right.z;
    lookat[2][1] = Camup.z;
    lookat[2][2] = Direction.z;
    lookat[3][0] = - glm::dot(eye, Right);
    lookat[3][1] = - glm::dot(eye, Camup);
    lookat[3][2] = - glm::dot(eye, Direction);
    return lookat;

}


float Camera::GetZoom()
{
    return Zoom;
}