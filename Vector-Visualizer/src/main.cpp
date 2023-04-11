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
#include "VectorObjectUtils.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Shader.h"
#include "renderer/Camera.h"

#include "modes/ModeMenu.h"
#include "modes/ModeVectorMultiple.h"
#include "modes/ModeVectorTransformation.h"

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

    // vectorMenu setup
    std::shared_ptr<std::vector<VectorObject>> vectorMenu(new std::vector<VectorObject>);

    std::shared_ptr<std::vector<float>> vectorMenuBuffer(new std::vector<float>);

    std::shared_ptr<VertexArray> vectorMenuVA(new VertexArray);
    std::shared_ptr<VertexBuffer> vectorMenuVB(new VertexBuffer(vectorMenuBuffer->data(), sizeof(float) * vectorMenuBuffer->size(), MODE::DYNAMIC));
    // bind vertex buffer to vertex array
    vectorMenuVA->AddBuffer(*vectorMenuVB, layout);

    // vectorMultiple setup
    VectorObject vectorMultiple1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 4.0f, -5.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    std::shared_ptr<std::vector<VectorObject>> vectorMultiple(new std::vector<VectorObject>);
    vectorMultiple->push_back(vectorMultiple1);

    std::shared_ptr<std::vector<float>> vectorMultipleBuffer(new std::vector<float>);

    for (VectorObject vec : *vectorMultiple)
    {
        AddVectorBufferData(*vectorMultipleBuffer, vec);
    }

    std::shared_ptr<VertexArray> vectorMultipleVA(new VertexArray);
    std::shared_ptr<VertexBuffer> vectorMultipleVB(new VertexBuffer(vectorMultipleBuffer->data(), sizeof(float) * vectorMultipleBuffer->size(), MODE::DYNAMIC));
    // bind vertex buffer to vertex array
    vectorMultipleVA->AddBuffer(*vectorMultipleVB, layout);

    // vectorTransform setup
    VectorObject vectorTransform1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 4.0f, -5.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    std::shared_ptr<std::vector<VectorObject>> vectorTransform(new std::vector<VectorObject>);
    vectorTransform->push_back(vectorTransform1);

    std::shared_ptr<std::vector<float>> vectorTransformBuffer(new std::vector<float>);

    for (VectorObject vec : *vectorTransform)
    {
        AddVectorBufferData(*vectorTransformBuffer, vec);
    }

    std::shared_ptr<VertexArray> vectorTransformVA(new VertexArray);
    std::shared_ptr<VertexBuffer> vectorTransformVB(new VertexBuffer(vectorTransformBuffer->data(), sizeof(float) * vectorTransformBuffer->size(), MODE::DYNAMIC));
    // bind vertex buffer to vertex array
    vectorTransformVA->AddBuffer(*vectorTransformVB, layout);

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

    displayMode::ModeMenu* modeMenu = new displayMode::ModeMenu(nullptr, vectorMenu, vectorMenuBuffer, vectorMenuVA, vectorMenuVB);
    modeMenu->m_CurrentMode = modeMenu;

    // register different modes with respective (vectors, vectorBuffer, vectorVA, vectorVB)
    modeMenu->RegisterMode<displayMode::ModeVectorMultiple>(vectorMultiple, vectorMultipleBuffer, vectorMultipleVA, vectorMultipleVB, "Multiple Vectors");
    modeMenu->RegisterMode<displayMode::ModeVectorTransformation>(vectorTransform, vectorTransformBuffer, vectorTransformVA, vectorTransformVB, "Matrix Transformation");

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

        /* Render here */
        axesVA.Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * 6);

        modeMenu->m_CurrentMode->m_VectorVA->Bind();
        shader.Bind();
        glDrawArrays(GL_LINES, 0, 2 * modeMenu->m_CurrentMode->m_VectorBuffer->size());

        // imgui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // mode switching
        modeMenu->m_CurrentMode->OnUpdate(0.0f);
        modeMenu->m_CurrentMode->OnRender();
        ImGui::Begin("Modes");
        if (modeMenu->m_CurrentMode != modeMenu && ImGui::Button("<- Back to mode selection"))
        {
            delete modeMenu->m_CurrentMode;
            modeMenu->m_CurrentMode = modeMenu;
        }
        modeMenu->m_CurrentMode->OnImGuiRender();
        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(windowID);

        /* Poll for and process events */
        glfwPollEvents();
    }
    // mode clean up
    delete modeMenu->m_CurrentMode;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    window.~Window();
    return 0;
}