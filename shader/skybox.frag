#version 330 core

in vec3 fTextureDir;

uniform samplerCube texture_cubemap0;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture_cubemap0, fTextureDir);
}