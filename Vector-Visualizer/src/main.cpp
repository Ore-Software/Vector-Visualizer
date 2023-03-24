#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>

#include "Window.h"
#include "renderer/VertexBuffer.h"
#include "renderer/Shader.h"

int main()
{
    Window window(1280, 720, "Vector Visualizer", NULL);

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
        0.0f, 0.0f, 0.0f,
        20.0f,  5.0f, 0.0f,
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

    // camera
    glm::vec3 cameraPosition = { 20.0f, 20.0f, 20.0f };
    glm::vec3 targetPosition = { 0.0f, 0.0f, 0.0f };

    glm::vec3 direction = cameraPosition - targetPosition;
    glm::normalize(direction);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, direction));
    glm::vec3 cameraUp = glm::cross(direction, cameraRight);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, targetPosition, cameraUp);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;

    // upload uniforms
    axesShader.Bind();
    axesShader.SetUniformMat4f("u_MVP", MVP);
    vectorShader.Bind();
    vectorShader.SetUniformMat4f("u_MVP", MVP);

    // openGL settings
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window.GetID()))
    {
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
        glfwSwapBuffers(window.GetID());

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}