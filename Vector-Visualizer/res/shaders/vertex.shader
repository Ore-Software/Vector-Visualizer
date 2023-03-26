#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);
    v_Color = color;
};