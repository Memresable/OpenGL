/* date = July 6th 2022 4:59 pm */

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Camera
{
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float yaw{};
    float pitch{};
    float fov = 45.0f;
    
    float lastX = 400, lastY = 300;
    
    bool mouseFirst{};
    
};

Camera camera;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.fov -= (float)yoffset;
    if(camera.fov < 45.0f)
        camera.fov = 45.0f;
    if(camera.fov > 90.0f)
        camera.fov = 90.0f;
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
    if(camera.mouseFirst)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.mouseFirst = false;
    }
    
    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;
    camera.lastX = xpos;
    camera.lastY = ypos;
    
    const float sensitivty = 0.1f;
    xoffset *= sensitivty;
    yoffset *= sensitivty;
    
    camera.yaw   += xoffset;
    camera.pitch += yoffset;
    
    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;
    
    glm::vec3 direction{};
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.cameraFront = glm::normalize(direction);
}

#endif //CAMERA_H
