#version 330 core
in vec4 fColor;
in vec2 fUv;
in vec3 fFragPos;
in vec3 fNormal;

uniform sampler2D texture_none0;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(texture(texture_none0, fUv)), 1.0);
}