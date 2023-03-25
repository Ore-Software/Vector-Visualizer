#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>

#include "Window.h"
#include "renderer/VertexBuffer.h"
#include "renderer/Shader.h"

int main()
{
    float screenWidth = 1280.0f;
    float screenHeight = 720.0f;
    float aspectRatio = screenWidth / screenHeight;
    Window window(screenWidth, screenHeight, "Vector Visualizer", NULL);

    // axes setup
    static const float axes[]
    {
        -10.0f,  0.0f, 0.0f, // x axis
        10.0f,  0.0f, 0.0f,
        0.0f,  -10.0f, 0.0f, // y axis
        0.0f,  10.0f, 0.0f,
        0.0f,  0.0f, -10.0f, // z axis
        0.0f,  0.0f, 10.0f
    };

    unsigned int axesVA;
    glGenVertexArrays(1, &axesVA);
    glBindVertexArray(axesVA);

    VertexBuffer axesVB(axes, sizeof(axes));

    // bind vertex buffer to vertex array
    glEnableVertexArrayAttrib(axesVA, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // vector setup
    static const float vector[]
    {
        0.0f, 0.0f, 0.0f,    // origin
        20.0f,  5.0f, 0.0f,  // endpoint
    };

    unsigned int vectorVA;
    glGenVertexArrays(1, &vectorVA);
    glBindVertexArray(vectorVA);

    VertexBuffer vectorVB(vector, sizeof(vector));

    // bind vertex buffer to vertex array
    glEnableVertexArrayAttrib(vectorVA, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // shaders
    std::string vertexFilepath = "res/shaders/vertex.shader";
    std::string axesFragmentFilepath = "res/shaders/axesFragment.shader";
    std::string vectorFragmentFilepath = "res/shaders/vectorFragment.shader";

    Shader axesShader(vertexFilepath, axesFragmentFilepath);
    Shader vectorShader(vertexFilepath, vectorFragmentFilepath);

    // camera setup
    glm::vec3 cameraPosition = { 5.0f, 5.0f, 5.0f };
    glm::vec3 cameraFront = { 0.0f, 0.0f, -1.0f };

    glm::vec3 unitcameraFront = glm::normalize(cameraFront);

    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, unitcameraFront));
    glm::vec3 cameraUp = glm::cross(unitcameraFront, cameraRight);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

    glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;

    // upload uniforms
    axesShader.Bind();
    axesShader.SetUniformMat4f("u_MVP", MVP);
    vectorShader.Bind();
    vectorShader.SetUniformMat4f("u_MVP", MVP);

    // openGL settings
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0f);

    GLFWwindow* windowID = window.GetID();

    // keyboard movement variables
    double currentTime = 0.0;
    double lastTime = 0.0;
    float deltaTime = 0.0;
    float forwardSpeed = 5.0f;
    float strafeSpeed = 50.0;

    // mouse movement variables
    double lastXpos, lastYpos, currXpos, currYpos, deltaX, deltaY;
    glfwGetCursorPos(windowID, &currXpos, &currYpos);
    glm::vec3 angleDirection;
    double yaw = 0.0;
    double pitch = 0.0;
    double sens = 1.0/720.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(windowID))
    {
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // camera updates per frame
        unitcameraFront = glm::normalize(cameraFront);

        cameraRight = glm::normalize(glm::cross(worldUp, unitcameraFront));
        cameraUp = glm::cross(unitcameraFront, cameraRight);

        modelMatrix = glm::mat4(1.0f);
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

        MVP = projMatrix * viewMatrix * modelMatrix;

        // upload uniforms
        axesShader.Bind();
        axesShader.SetUniformMat4f("u_MVP", MVP);
        vectorShader.Bind();
        vectorShader.SetUniformMat4f("u_MVP", MVP);

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

        //// angles
        //if (glfwGetKey(windowID, GLFW_KEY_UP) == GLFW_PRESS)
        //{
        //    pitch += 1.0;
        //    direction.x -= glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
        //    direction.y -= glm::sin(glm::radians(pitch));
        //    direction.z -= glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
        //}
        //// Strafe left
        //if (glfwGetKey(windowID, GLFW_KEY_LEFT) == GLFW_PRESS)
        //{
        //    
        //}
        //// Move backward
        //if (glfwGetKey(windowID, GLFW_KEY_DOWN) == GLFW_PRESS)
        //{
        //    pitch -= 1.0;
        //    direction.x += glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
        //    direction.y += glm::sin(glm::radians(pitch));
        //    direction.z += glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
        //}
        //// Strafe right
        //if (glfwGetKey(windowID, GLFW_KEY_RIGHT) == GLFW_PRESS)
        //{
        //    
        //}

        lastXpos = currXpos;
        lastYpos = currYpos;
        glfwGetCursorPos(windowID, &currXpos, &currYpos);
        yaw += sens * deltaTime * float(currXpos - lastXpos);
        pitch += sens * deltaTime * float(currYpos - lastYpos);


        glm::vec3 moveDirection(
            cos(pitch) * sin(yaw),
            sin(pitch),      
            cos(pitch) * cos(yaw)
        );

        glm::vec3 right = glm::vec3(
            sin(yaw - 3.14f / 2.0f),
            0,
            cos(yaw - 3.14f / 2.0f)
        );

        glm::vec3 newUp = glm::cross(right, moveDirection);

        // rotate -z
        if (glfwGetKey(windowID, GLFW_KEY_UP) == GLFW_PRESS) {
            cameraFront -= moveDirection * deltaTime * 5.0f;

        }
        // rotate +z
        if (glfwGetKey(windowID, GLFW_KEY_DOWN) == GLFW_PRESS) {
            cameraFront += moveDirection * deltaTime * 5.0f;

        }
        // rotate +x
        if (glfwGetKey(windowID, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cameraFront -= right * deltaTime * 5.0f;

        }
        // rotate -x
        if (glfwGetKey(windowID, GLFW_KEY_LEFT) == GLFW_PRESS) {
            cameraFront += right * deltaTime * 5.0f;

        }

        // mouse movement
        //if (glfwGetMouseButton(windowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        //{
        //    // consider infinite dragging (unrestricted by window border) by getting/setting position later
        //    
        //    deltaX = currXpos - lastXpos;
        //    deltaY = currYpos - lastYpos;
        //    
        //    yaw -= deltaX * sens;
        //    pitch += deltaY * sens;

        //    direction.x += glm::cos(yaw);
        //    direction.z -= glm::sin(yaw);
        //    direction.y += glm::sin(pitch);

        //}
        

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* Render here */
        glBindVertexArray(axesVA);
        axesShader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 3);

        glBindVertexArray(vectorVA);
        vectorShader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 1);

        /* Swap front and back buffers */
        glfwSwapBuffers(windowID);

        /* Poll for and process events */
        glfwPollEvents();
    }

    window.~Window();
    return 0;
}