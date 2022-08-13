#version 330 core
in vec3 fColor;
in vec2 fUv;

uniform sampler2D uTexture;
uniform int uUseTexture;
out vec4 FragColor;

void main()
{
    if (uUseTexture != 0)
        FragColor = vec4(texture(uTexture, fUv).rgb, 1.0);
    else
        FragColor = vec4(fColor, 1.0);
}