#version 460

layout(location = 0) in vec3 vertex_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 sprite_color;

out vec3 color;

void main()
{
    color = sprite_color;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}