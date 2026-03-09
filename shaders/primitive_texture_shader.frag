#version 460 core

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 sprite_color;

out vec4 FragColor;

void main()
{
    vec4 tex1 = texture(texture1, TexCoord);
    vec4 tex2 = texture(texture2, TexCoord);
    vec4 mixed = mix(tex1, tex2, 0.2);

    FragColor = mixed * vec4(sprite_color);
}