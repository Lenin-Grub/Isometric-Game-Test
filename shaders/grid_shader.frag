#version 460 core

in vec3 v_world_pos;
layout(location = 0) out vec4 frag_color;

uniform float grid_step;
uniform vec3  grid_color;

void main()
{
    float x_dist = abs(mod(v_world_pos.x + grid_step/2.0, grid_step) - grid_step/2.0);
    float y_dist = abs(mod(v_world_pos.y + grid_step/2.0, grid_step) - grid_step/2.0);

    float dx = fwidth(v_world_pos.x);
    float dy = fwidth(v_world_pos.y);
    float line_width_x = 1.0 * dx;
    float line_width_y = 1.0 * dy;

    if (x_dist < line_width_x || y_dist < line_width_y)
    {
        if (abs(v_world_pos.y) < line_width_y && abs(v_world_pos.z) < 0.01)
        {
            frag_color = vec4(1.0, 0.0, 0.0, 1.0);
        }
        else if (abs(v_world_pos.x) < line_width_x && abs(v_world_pos.z) < 0.01)
        {
            frag_color = vec4(0.0, 1.0, 0.0, 1.0);
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