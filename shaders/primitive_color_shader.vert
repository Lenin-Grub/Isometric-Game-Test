#version 460

layout (location = 0 )in vec3 vertex_position;
layout (location = 1 )in vec3 vertex_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec3 color;

void main()
{
    color = vertex_color;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}