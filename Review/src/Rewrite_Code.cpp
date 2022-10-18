#include "Main_include.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{

}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
    static float lastframe{0.0f};
    static float currentframe{0.0f};
    static float deltatime{0.0f};
    static int state{GLFW_CURSOR_NORMAL};

    currentframe = (float)glfwGetTime();
    deltatime = currentframe - lastframe;
    lastframe = currentframe;

    Camera* camera = (Camera*)(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {glfwSetWindowShouldClose(window, true);}
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, state);
        state = (state == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::FORWARD, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::BACKWARD, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::LEFT, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::RIGHT, deltatime);}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = (Camera*)(glfwGetWindowUserPointer(window));
    camera->ZoomCamera(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
}