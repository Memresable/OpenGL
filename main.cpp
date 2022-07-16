#include "model.h"
#include "triangle.h"
#include "camera.h"

#include <iostream>

// Resolution
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    // GLFW Settings + Mouse Settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MemreOpenGL", NULL, NULL);
    
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallBack);
    glfwSetScrollCallback(window, scroll_callback);
    
    // OpenGL Startup
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
    
    // Load the Shader
    Shader shader("vertex.shader", "fragment.shader");
    shader.use();
    
    // Load the Model
    Model model("backpack/backpack.obj");
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glClearColor(0.0f, 0.25f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Camera
        float radius = 5.0f;
        float camX = sin((float)glfwGetTime()) * radius;
        float camZ = cos((float)glfwGetTime()) * radius;
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(camera.cameraPosition,
                           camera.cameraPosition + camera.cameraFront,
                           camera.cameraUp);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 10000.0f);
        
        lightPosition.x = 1.0f + sin((float)glfwGetTime()) * 4.0f;
        lightPosition.y = sin((float)glfwGetTime() / 2) * 2.0f;
        
        // Shader Properties
        shader.use();
        shader.setMat4p("View", view);
        shader.setMat4p("Projection", projection);
        shader.setVec3("objectColor", containerColor);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("lightPos", camera.cameraPosition);
        shader.setVec3("viewPos", camera.cameraPosition);
        
        shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        
        shader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        
        shader.setVec3("pointLights[0].position", lightPosition);
        shader.setVec3("pointLights[0].direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.setVec3("pointLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("pointLights[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.0032f);
        
        shader.setVec3("spotLight.position", camera.cameraPosition);
        shader.setVec3("spotLight.direction", camera.cameraFront);
        shader.setVec3("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
        shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outercutoff", glm::cos(glm::radians(17.5f)));
        
        // Position of the module
        glm::mat4 v_model = glm::mat4(1.0f);
        v_model = glm::translate(v_model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4p("Model", v_model);
        
        //Draw
        model.Draw(shader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // Close the Window ("Escape" Key)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Camera Movement and Speed
    float currentFrame = glfwGetTime(); 
    camera.deltaTime = currentFrame - camera.lastFrame;
    camera.lastFrame = currentFrame;
    
    const float cameraSpeed = 10.5f * camera.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.cameraPosition += cameraSpeed * camera.cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.cameraPosition -= cameraSpeed * camera.cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.cameraPosition -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.cameraPosition += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    
    // Polygon Mode
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}