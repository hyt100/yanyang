#version 330 core
layout (location = 0) in vec3 vPos;

uniform mat4 mMat;
uniform mat4 mvMat;
uniform mat4 mvpMat;
uniform mat3 normalMat;

out vec3 fTextureDir;

void main()
{
  fTextureDir = mat3(mMat) * vPos;
  vec4 pos = mvpMat * vec4(vPos, 1.0);
  gl_Position = pos.xyww;
}
