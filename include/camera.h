#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

float mixValue = 0.2f, deltaTime = 0.0f, lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraTar = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFro = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraDir = glm::normalize(cameraPos - cameraTar);
glm::vec3 up        = glm::vec3(0.0f, 0.1f, 0.0f);
glm::vec3 cameraRig = glm::normalize(glm::cross(up, cameraDir));
glm::vec3 cameraUp  = glm::cross(cameraDir, cameraRig);

bool firstMouse = true;
float yaw = -90.0f, pitch = 0.0f; float lastX = 800.0f / 2.0, lastY = 600.0 / 2.0; float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn), ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos; lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX, yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity; yoffset *= sensitivity;

    yaw += xoffset; pitch += yoffset;

    if (pitch > 89.0f ) { pitch = 89.0f;  }
    if (pitch < -89.0f) { pitch = -89.0f; }

    glm::vec3 front{};
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFro = glm::normalize(front);
}

#endif