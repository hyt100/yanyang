#version 330 core
in vec3 fNormal;
in vec2 fUv;

uniform sampler2D texture_diffuse0;
out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(texture_diffuse0, fUv).rgb, 1.0);
}