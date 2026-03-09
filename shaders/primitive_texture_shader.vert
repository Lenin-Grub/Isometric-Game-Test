#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 sprite_color;

void main()
{
	color       = sprite_color;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord    = vec2(aTexCoord.x, aTexCoord.y);
}