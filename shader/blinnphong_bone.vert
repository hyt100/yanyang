#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vUv;
layout (location = 6) in vec4 vBoneID;
layout (location = 7) in vec4 vBoneWeight;

const int MAX_BONES = 200;         // mesh最多允许的骨骼数量
const int MAX_BONE_INFLUENCE = 4;  // 影响每个顶点的骨骼最多数量(如果真实情况不止四个，我们也只取四个)
uniform mat4 boneMat[MAX_BONES];

uniform mat4 mMat;
uniform mat4 mvMat;
uniform mat4 mvpMat;
uniform mat3 normalMat;

out vec4 fColor;
out vec3 fNormal;
out vec2 fUv;
out vec3 fFragPos;

void main()
{
  vec4 skinPos = vec4(0.0);
  for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
    if (vBoneID[i] < 0) { // 无效的骨骼ID，无需处理
      continue;
    }
    if (vBoneID[i] >= MAX_BONES) {  // 如果骨骼数超过我们的最大值，说明无法处理了
      skinPos = vec4(vPos, 1.0);
      break;
    }

    skinPos += vec4(vBoneWeight[i], vBoneWeight[i], vBoneWeight[i], 1.0) * boneMat[vBoneID[i]] * vec4(vPos, 1.0);
  }

  gl_Position = mvpMat * skinPos;
  fFragPos = vec3(mvMat * vec4(vPos, 1.0));
  fNormal = normalMat * vNormal;
  fColor = vColor;
  fUv = vUv;
}