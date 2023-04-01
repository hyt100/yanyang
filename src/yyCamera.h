#pragma once
#include "yyUtil.h"

class yyCamera
{
public:
    using Ptr = std::shared_ptr<yyCamera>;

    glm::mat4 projectionMat_;
    glm::mat4 viewMat_;
    glm::mat4 viewProjectionMat_;
    glm::vec3 position_; // 相机在世界坐标系中的位置信息
    glm::vec3 target_;   // 相机注视点
    glm::vec3 up_;       // 相机up方向向量(归一化)
};
