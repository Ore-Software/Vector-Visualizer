#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/random.hpp"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include <vector>

#include "Window.h"
#include "VectorObject.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Shader.h"
#include "renderer/Camera.h"

// function prototypes, implementations at bottom
void AddVectorBufferData(std::vector<float>& buffer, VectorObject vectorObject);
void EditVectorBufferData(std::vector<float>& buffer, const std::vector<VectorObject>& vectorObjects, unsigned int index);

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
        0.0f,  0.0f, 0.0f, // negative x axis
        0.95f, 0.13f, 0.11f, 1.0f,
        -10.0f,  0.0f, 0.0f,
        0.95f, 0.13f, 0.11f, 1.0f,
        0.0f,  0.0f, 0.0f, // positive y axis
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  10.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // negative y axis
        0.10f, 0.83f, 0.16f, 1.0f,
        0.0f, -10.0f, 0.0f,
        0.10f, 0.83f, 0.16f, 1.0f,
        0.0f,  0.0f, 0.0f, // positive z axis 
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f,  0.0f, 10.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f,  0.0f, 0.0f, // negative z axis
        0.13f, 0.26f, 0.98f, 1.0f,
        0.0f,  0.0f, -10.0f,
        0.13f, 0.26f, 0.98f, 1.0f,
    };
    
    VertexArray axesVA;
    VertexBuffer axesVB(axes, sizeof(axes), MODE::STATIC);
    // bind vertex buffer to vertex array
    axesVA.AddBuffer(axesVB, layout);

    // vector setup
    VectorObject vector1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 4.0f, -5.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    //VectorObject vector3();

    std::vector<VectorObject> vectors;
    vectors.push_back(vector1);

    std::vector<float> vectorBuffer;

    for (VectorObject vec : vectors)
    {
        AddVectorBufferData(vectorBuffer, vec);
    }

    VertexArray vectorVA;
    VertexBuffer vectorVB(vectorBuffer.data(), sizeof(float) * vectorBuffer.size(), MODE::DYNAMIC);
    // bind vertex buffer to vertex array
    vectorVA.AddBuffer(vectorVB, layout);

    // transformation matrix
    float transMatrix[]
    {
        1.0f,  0.0f, 0.0f, // row 1
        0.0f,  1.0f, 0.0f, // row 2
        0.0f,  0.0f, 1.0f, // row 3
    };

    // shaders
    std::string vertexFilepath = "res/shaders/vertex.shader";
    std::string fragmentFilepath = "res/shaders/fragment.shader";

    Shader shader(vertexFilepath, fragmentFilepath);

    // camera setup
    double yaw = -135.0; // initially looks at the origin
    double pitch = -30.0;
    glm::vec3 cameraPosition = { 5.0f, 5.0f, 5.0f };
    Camera camera(cameraPosition, yaw, pitch);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

    // upload uniforms
    shader.Bind();
    shader.SetUniformMat4f("u_Model", modelMatrix);
    shader.SetUniformMat4f("u_View", camera.GetViewMatrix());
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
    double currXpos, currYpos, deltaX, deltaY;
    double lastXpos = 0.0;
    double lastYpos = 0.0;
    double sens = 15.0;


    // Setup Dear ImGui context
    ImGui::CreateContext();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(windowID, true);
    ImGui_ImplOpenGL3_Init();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(windowID))
    {
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // camera updates per frame
        camera.LookAt(yaw, pitch);
        camera.SetViewMatrix();

        // upload uniforms
        shader.Bind();
        shader.SetUniformMat4f("u_View", camera.GetViewMatrix());

        // keyboard input
        // Move forward
        if (glfwGetKey(windowID, GLFW_KEY_W) == GLFW_PRESS)
        {
            /*cameraPosition += cameraFront * deltaTime * forwardSpeed;*/
            camera.MoveCamera(camera.GetCameraFront(), deltaTime * forwardSpeed);
        }
        // Move backward
        if (glfwGetKey(windowID, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.MoveCamera(-camera.GetCameraFront(), deltaTime * forwardSpeed);
        }
        // Strafe left
        if (glfwGetKey(windowID, GLFW_KEY_A) == GLFW_PRESS)
        {
            /*cameraPosition += cameraRight * deltaTime * strafeSpeed;*/
            camera.MoveCamera(camera.GetCameraRight(), deltaTime* forwardSpeed);
        }
        // Strafe right
        if (glfwGetKey(windowID, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.MoveCamera(-camera.GetCameraRight(), deltaTime * forwardSpeed);
        }
        // fly up
        if (glfwGetKey(windowID, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera.MoveCamera(camera.GetCameraUp(), deltaTime * forwardSpeed);
        }
        // drop down
        if (glfwGetKey(windowID, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera.MoveCamera(-camera.GetCameraUp(), deltaTime * forwardSpeed);
        }

        // pitch up
        if (glfwGetKey(windowID, GLFW_KEY_UP) == GLFW_PRESS)
        {
            pitch += sens * deltaTime;
        }
        // pitch down
        if (glfwGetKey(windowID, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            pitch -= sens * deltaTime;
        }
        // yaw left
        if (glfwGetKey(windowID, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            yaw -= sens * deltaTime;
        }
        // yaw right
        if (glfwGetKey(windowID, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            yaw += sens * deltaTime;
        }

        // mouse movement

        glfwGetCursorPos(windowID, &currXpos, &currYpos);
        deltaX = (currXpos - lastXpos) / screenWidth;  // it is bounded by -1 and 1
        deltaY = (currYpos - lastYpos) / screenHeight; // it is bounded by -1 and 1
        lastXpos = currXpos;
        lastYpos = currYpos;

        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(windowID, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            yaw -= deltaX * sens;
            pitch += deltaY * sens;
        }

        // clearing per frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // imgui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        axesVA.Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 6);

        vectorVA.Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * vectors.size());

        // imgui vector controls
        ImGui::Begin("Vector Controls");
        for (int j = 0; j < vectors.size(); j++)
        {
            ImGui::PushID(j);
            ImGui::Text("Vector %d", j + 1);
            ImGui::SliderFloat3("Origin", &vectors[j].m_Origin.x, -10.0f, 10.0f);
            ImGui::SliderFloat3("Direction", &vectors[j].m_Direction.x, -10.0f, 10.0f);
            ImGui::ColorEdit4("Color", &vectors[j].m_Color.x);
            if (ImGui::Button("Apply Changes"))
            {
                EditVectorBufferData(vectorBuffer, vectors, j);

                vectorVA.Bind();
                vectorVB.Bind();
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
            };

            if (ImGui::Button("Remove vector"))
            {
                vectors.erase(vectors.begin() + j);
                
                // reconstruct buffer of vector vertices
                vectorBuffer.clear();
                for (VectorObject vec : vectors)
                {
                    AddVectorBufferData(vectorBuffer, vec);
                }

                vectorVA.Bind();
                vectorVB.Bind();
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::PopID();
        }
        if (ImGui::Button("Add vector"))
        {
            VectorObject newVec(glm::vec3(0.0f, 0.0f, 0.0f), glm::ballRand(5.0f), glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
            vectors.push_back(newVec);
            AddVectorBufferData(vectorBuffer, newVec);

            vectorVA.Bind();
            vectorVB.Bind();
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
        }
        ImGui::End();
        
        // imgui vector controls
        ImGui::Begin("Matrix Transformation");
        ImGui::Text("Matrix entries");
        ImGui::SliderFloat3("Row 1", &transMatrix[0], -10.0f, 10.0f);
        ImGui::SliderFloat3("Row 2", &transMatrix[3], -10.0f, 10.0f);
        ImGui::SliderFloat3("Row 3", &transMatrix[6], -10.0f, 10.0f);
        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(windowID);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    window.~Window();
    return 0;
}

void AddVectorBufferData(std::vector<float>& buffer, VectorObject vectorObject)
{
    buffer.push_back(vectorObject.m_Origin.x);
    buffer.push_back(vectorObject.m_Origin.y);
    buffer.push_back(vectorObject.m_Origin.z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);

    buffer.push_back(vectorObject.m_Direction.x);
    buffer.push_back(vectorObject.m_Direction.y);
    buffer.push_back(vectorObject.m_Direction.z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);
}

void EditVectorBufferData(std::vector<float>& buffer, const std::vector<VectorObject>& vectorObjects, unsigned int index)
{
    buffer[14 * index + 0] = vectorObjects[index].m_Origin.x;
    buffer[14 * index + 1] = vectorObjects[index].m_Origin.y;
    buffer[14 * index + 2] = vectorObjects[index].m_Origin.z;

    buffer[14 * index + 3] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 4] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 5] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 6] = vectorObjects[index].m_Color.w;

    buffer[14 * index + 7] = vectorObjects[index].m_Direction.x;
    buffer[14 * index + 8] = vectorObjects[index].m_Direction.y;
    buffer[14 * index + 9] = vectorObjects[index].m_Direction.z;

    buffer[14 * index + 10] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 11] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 12] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 13] = vectorObjects[index].m_Color.w;
}