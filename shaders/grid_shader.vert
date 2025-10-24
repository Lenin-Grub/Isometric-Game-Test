#version 460 core

uniform mat4 grid_mvp;

const vec3 positions[4] = vec3[4](
    vec3(-1.0, -1.0, 0.0),
    vec3( 1.0, -1.0, 0.0),
    vec3( 1.0,  1.0, 0.0),
    vec3(-1.0,  1.0, 0.0)
);

const int indices[6] = int[6](0, 1, 2, 0, 2, 3);

out vec3 v_world_pos;

void main()
{
    int index = indices[gl_VertexID];
    vec3 pos = positions[index];
    v_world_pos = pos;
    gl_Position = grid_mvp * vec4(pos, 1.0);
}