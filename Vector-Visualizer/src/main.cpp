#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>

#include "Window.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Shader.h"

int main()
{
    float screenWidth = 1280.0f;
    float screenHeight = 720.0f;
    float aspectRatio = screenWidth / screenHeight;
    Window window(screenWidth, screenHeight, "Vector Visualizer", NULL);

    VertexBufferLayout layout;
    layout.Push<float>(3); // 3d coordinates
    layout.Push<float>(4); // colors

    // axes setup
    static const float axes[]
    {
        0.0f,  0.0f, 0.0f, // positive x axis
        1.0f, 0.0f, 0.0f, 1.0f, 
        10.0f,  0.0f, 0.0f, 
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // negative x axis (TODO: color is different numerically, but not visually)
        0.8f, 0.0f, 0.0f, 1.0f,
        -10.0f,  0.0f, 0.0f,
        0.8f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // positive y axis
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  10.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // negative y axis (TODO: color is different numerically, but not visually)
        0.0f, 0.8f, 0.0f, 1.0f,
        0.0f, -10.0f, 0.0f,
        0.0f, 0.8f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // positive z axis 
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f,  0.0f, 10.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // negative z axis (TODO: color is different numerically, but not visually)
        0.0f, 0.0f, 0.8f, 1.0f,
        0.0f,  0.0f, -10.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
    
    VertexArray axesVA;
    VertexBuffer axesVB(axes, sizeof(axes));
    // bind vertex buffer to vertex array
    axesVA.AddBuffer(axesVB, layout);

    // vector setup
    static const float vector[]
    {
        0.0f, 0.0f, 0.0f,    // origin
        1.0f, 0.0f, 1.0f, 1.0f,
        20.0f,  5.0f, 0.0f,  // endpoint
        1.0f, 0.0f, 1.0f, 1.0f
    };

    VertexArray vectorVA;
    VertexBuffer vectorVB(vector, sizeof(vector));
    // bind vertex buffer to vertex array
    vectorVA.AddBuffer(vectorVB, layout);

    // shaders
    std::string vertexFilepath = "res/shaders/vertex.shader";
    std::string fragmentFilepath = "res/shaders/fragment.shader";

    Shader shader(vertexFilepath, fragmentFilepath);

    // camera setup
    double yaw = -90.0;
    double pitch = 0.0;
    glm::vec3 cameraPosition = { 5.0f, 5.0f, 5.0f };
    glm::vec3 cameraFront =
    {
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    };

    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));
    glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

    // upload uniforms
    shader.Bind();
    shader.SetUniformMat4f("u_Model", modelMatrix);
    shader.SetUniformMat4f("u_View", viewMatrix);
    shader.SetUniformMat4f("u_Projection", projMatrix);

    // openGL settings
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0f);

    GLFWwindow* windowID = window.GetID();

    // keyboard movement variables
    double currentTime = 0.0;
    double lastTime = 0.0;
    float deltaTime = 0.0f;
    float forwardSpeed = 5.0f;
    float strafeSpeed = 50.0f;

    // mouse movement variables
    double centerX = screenWidth / 2;
    double centerY = screenHeight / 2;
    double currXpos, currYpos, deltaX, deltaY;
    double lastXpos = 0.0;
    double lastYpos = 0.0;
    double sens = 15.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(windowID))
    {
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // camera updates per frame
        cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront.y = sin(glm::radians(pitch));
        cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));
        cameraUp = glm::cross(cameraFront, cameraRight);

        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        // upload uniforms
        shader.Bind();
        shader.SetUniformMat4f("u_View", viewMatrix);

        // keyboard input
        // Move forward
        if (glfwGetKey(windowID, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPosition += cameraFront * deltaTime * forwardSpeed;
        }
        // Strafe left
        if (glfwGetKey(windowID, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition += cameraRight * deltaTime * strafeSpeed;
        }
        // Move backward
        if (glfwGetKey(windowID, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPosition -= cameraFront * deltaTime * forwardSpeed;
        }
        // Strafe right
        if (glfwGetKey(windowID, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPosition -= cameraRight * deltaTime * strafeSpeed;
        }
        // fly up
        if (glfwGetKey(windowID, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPosition += cameraUp * deltaTime * strafeSpeed;
        }
        // drop down
        if (glfwGetKey(windowID, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPosition -= cameraUp * deltaTime * strafeSpeed;
        }

        // pitch up
        if (glfwGetKey(windowID, GLFW_KEY_UP) == GLFW_PRESS)
        {
            pitch += sens * deltaTime;
        }
        // yaw left
        if (glfwGetKey(windowID, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            yaw -= sens * deltaTime;
        }
        // pitch down
        if (glfwGetKey(windowID, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            pitch -= sens * deltaTime;
        }
        // yaw right
        if (glfwGetKey(windowID, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            yaw += sens * deltaTime;
        }

        // mouse movement

        glfwGetCursorPos(windowID, &currXpos, &currYpos);
        deltaX = (currXpos - lastXpos) / screenWidth;
        deltaY = (currYpos - lastYpos) / screenHeight;
        lastXpos = currXpos;
        lastYpos = currYpos;

        if (glfwGetMouseButton(windowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            yaw -= deltaX * sens;
            pitch += deltaY * sens;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */
        axesVA.Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 6);

        vectorVA.Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 1);

        /* Swap front and back buffers */
        glfwSwapBuffers(windowID);

        /* Poll for and process events */
        glfwPollEvents();
    }

    window.~Window();
    return 0;
}