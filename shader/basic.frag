#version 330 core
in vec3 fNormal;
in vec4 fColor;
in vec2 fUv;

uniform sampler2D texture_diffuse0;
uniform int useVertexColor;
out vec4 FragColor;

void main()
{
    if (useVertexColor == 0)
        FragColor = vec4(texture(texture_diffuse0, fUv).rgb, 1.0);
    else
        FragColor = vec4(fColor);
}