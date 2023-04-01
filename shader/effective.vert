#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 3) in vec2 vUv;

uniform mat4 mMat;
uniform mat4 mvMat;
uniform mat4 mvpMat;
uniform mat3 normalMat;

out vec2 fUv;

void main()
{
  gl_Position = vec4(vPos.x, vPos.y, 0.0f, 1.0f);
  fUv = vUv;
}