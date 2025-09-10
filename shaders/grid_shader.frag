#version 460 core

in vec3 v_world_pos;
layout(location = 0) out vec4 frag_color;

uniform float grid_step;
uniform vec3  grid_color;

void main()
{
    float x_dist = abs(mod(v_world_pos.x + grid_step/2.0, grid_step) - grid_step/2.0);
    float z_dist = abs(mod(v_world_pos.z + grid_step/2.0, grid_step) - grid_step/2.0);

    float dx = fwidth(v_world_pos.x);
    float dz = fwidth(v_world_pos.z);
    float line_width_x = 1.5 * dx;
    float line_width_z = 1.5 * dz;

    if (x_dist < line_width_x || z_dist < line_width_z)
    {
        float dy = fwidth(v_world_pos.y);
        float line_width_y = 1.5 * dy;

        // I made mistake somewhere in axes
        // Axe X but it green like Y
        if (abs(v_world_pos.x) < line_width_x)
        {
            frag_color = vec4(0.0, 1.0, 0.0, 1.0);
        }
        // Axe Y — blue like Z but it should be green
        else if (abs(v_world_pos.y) < line_width_y)
        {
            frag_color = vec4(0.0, 0.0, 1.0, 1.0);
        }
        // Axe Z — red but it should be blue
        else if (abs(v_world_pos.z) < line_width_z)
        {
            frag_color = vec4(1.0, 0.0, 0.0, 1.0);
        }
        else
        {
            frag_color = vec4(grid_color, 1.0);
        }
    }
    else
    {
        discard;
    }
}