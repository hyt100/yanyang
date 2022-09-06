
#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 3) in vec2 vUv;

uniform mat4 mvp;

out vec3 fColor;
out vec2 fUv;

void main()
{
  gl_Position = mvp * vec4(vPos.x, vPos.y, vPos.z, 1.0);
  fColor = vColor;
  fUv = vUv;
}
