#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model_matrix;

void main()
{
	gl_Position = model_matrix * vec4(aPos, 1.0);
	TexCoord    = vec2(aTexCoord.x, aTexCoord.y);
}