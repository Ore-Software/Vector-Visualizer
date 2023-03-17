#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>

int main(void)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // show version
    //std::cout << glGetString(GL_VERSION) << "\n";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Vector Visualizer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Load all OpenGL functions using the glfw loader function
    // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1280.0f, 720.0f);

    std::string vertexShader = R"(#version 460 core
layout(location=0) in vec3 position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.0f);
};)";
    const char* vertexShaderCstr = vertexShader.c_str();

    std::string fragmentShader = R"(#version 460 core
layout(location=0) out vec4 color;
void main()
{
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};)";

    const char* fragmentShaderCstr = fragmentShader.c_str();

    unsigned int va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    unsigned int vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    static const float linePositions[]
    {
        0.0f, 0.0f, 0.0f, // vector
        20.0f,  5.0f, 0.0f,
        -10.0f,  0.0f, 0.0f, // x axis
        10.0f,  0.0f, 0.0f,
        0.0f,  -10.0f, 0.0f, // y axis
        0.0f,  10.0f, 0.0f,
        0.0f,  0.0f, -10.0f, // z axis
        0.0f,  0.0f, 10.0f
    };

    const float trianglePositions[]
    {
        -0.5f, 0.0f, 0.0f,
         0.5f, 0.0f, 0.0f,
         0.0f, 0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(linePositions), linePositions, GL_STATIC_DRAW);

    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    if (sizeof(linePositions) != size)
    {
        glDeleteBuffers(1, &vb);
        std::cout << "buffer error" << std::endl;
        return -1;
    }

    //bind vertex buffer to vertex array
    glEnableVertexArrayAttrib(va, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //shader
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vertexShaderCstr, NULL); // vertex_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(vshader);

    GLint vertex_compiled;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vshader, 1024, &log_length, message);
        // Write the error to a log
        std::cout << message << std::endl;
    }

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fragmentShaderCstr, NULL); // fragment_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(fshader);

    GLint fragment_compiled;
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fshader, 1024, &log_length, message);
        // Write the error to a log
        std::cout << message << std::endl;
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    glValidateProgram(program);

    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &log_length, message);
        // Write the error to a log
        std::cout << message << std::endl;
    }

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

    /*glm::mat4 projMatrix = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);*/

    glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;

    //bind shader
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "u_MVP"), 1, GL_FALSE, &MVP[0][0]);

    /*glEnable(GL_DEPTH_TEST);*/
    glLineWidth(5.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* Render here */
        glBindVertexArray(va);
        glDrawArrays(GL_LINES, 0, 2 * 4);
        /*glDrawArrays(GL_TRIANGLES, 0, 3);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}