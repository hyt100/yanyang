#pragma once
#include "yyUtil.h"

struct yyAmbientLight {
    glm::vec3 color;
    float intensity;
};

struct yyPointLight {
    // 光源位置
    glm::vec3 position;  

    // 光源颜色值
    glm::vec3 color;
    float intensity;

    // 光源衰减系数:  衰减系数attenuation = 1 / (constant + linear*d + quadratic*d*d)
    float constant;
    float linear;
    float quadratic;
};
