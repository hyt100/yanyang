#version 330 core
in vec2 fUv;

uniform sampler2D texture_none0;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(1.0 - texture(texture_none0, fUv)), 1.0);
}