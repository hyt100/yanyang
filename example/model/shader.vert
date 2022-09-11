
#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vUv;

uniform mat4 mvp;

out vec3 fNormal;
out vec2 fUv;

void main()
{
  gl_Position = mvp * vec4(vPos.x, vPos.y, vPos.z, 1.0);
  fNormal = vNormal;
  fUv = vUv;
}
