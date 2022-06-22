#include <iostream>
#include "include/shader.h"
#include "include/triangles_data.h"
#include "include/glfw3.h"
#include "include/camera.h"
#include "include/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);
//void load_texture(const char* t_name, const char* n_data); later?

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics_Engine", NULL, NULL);

    if (!(window))
    {
        std::cout << "GLFW Failed to create a window." << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
    {
        std::cout << "GLAD Failed to init!" << std::endl;
        glfwTerminate();
        return 0;
    }

    std::cout << glGetString(GL_VERSION) << '\n' << std::endl;

    // generate and bind the texture variable to "GL_TEXTURE_2D"
    stbi_set_flip_vertically_on_load(true);

    Shader shader("vertex.shader", "fragment.shader");
    Model backpack("backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    // render section
    while (!glfwWindowShouldClose(window))
    {
        proccessInput(window);

        glClearColor(1.0f, 0.8f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.p_use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.f, 0.1f, 100.0f);;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFro, cameraUp);
        shader.transformLoc("projection", projection);
        shader.transformLoc("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.transformLoc("model", model);

        backpack.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader.p_delete();

    glfwTerminate();
    return 0;
}

void proccessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    const float cameraSpe = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpe * cameraFro;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpe * cameraFro;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFro, cameraUp)) * cameraSpe;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFro, cameraUp)) * cameraSpe;
    }
}

//void load_texture(const char* t_name, const char* n_data)
//{
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load(t_name, &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load " << n_data << " Image Texture." << std::endl;
//    }
//
//    stbi_image_free(data);
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}