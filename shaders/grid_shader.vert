#version 460 core

layout(location = 0) in vec3 v_pos;

uniform mat4 grid_mvp;

out vec3 v_world_pos;

void main()
{
    v_world_pos = v_pos;
    gl_Position = grid_mvp * vec4(v_pos, 1.0);
}