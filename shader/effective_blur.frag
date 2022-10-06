#version 330 core
in vec4 fColor;
in vec2 fUv;
in vec3 fFragPos;
in vec3 fNormal;

uniform sampler2D texture_none0;
uniform int width;
uniform int height;

out vec4 FragColor;

void main()
{
    float offsetW = 1.0 / width;
    float offsetH = 1.0 / height;

    vec2 offsets[9] = vec2[](
        vec2(-offsetW,  offsetH), vec2( 0.0f,    offsetH), vec2( offsetW,  offsetH),
        vec2(-offsetW,  0.0f),    vec2( 0.0f,    0.0f),    vec2( offsetW,  0.0f),  
        vec2(-offsetW, -offsetH), vec2( 0.0f,   -offsetH), vec2( offsetW, -offsetH) 
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture_none0, fUv.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}