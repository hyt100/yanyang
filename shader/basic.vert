#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vColor;
layout (location = 3) in vec2 vUv;

uniform mat4 mMat;
uniform mat4 mvMat;
uniform mat4 mvpMat;
uniform mat3 normalMat;

out vec4 fColor;
out vec3 fNormal;
out vec2 fUv;

void main()
{
  gl_Position = mvpMat * vec4(vPos.x, vPos.y, vPos.z, 1.0);
  fNormal = normalMat * vNormal;
  fColor = vColor;
  fUv = vUv;
}
