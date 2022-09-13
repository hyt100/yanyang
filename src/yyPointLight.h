#pragma once
#include "yyUtil.h"

struct yyPointLight {
    // 光源位置
    glm::vec3 position;  

    // 光源颜色值(光源对环境光、漫反射和镜面光分量也分别具有不同的强度)
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // 光源衰减系数:  衰减系数attenuation = 1 / (constant + linear*d + quadratic*d*d)
    float constant;
    float linear;
    float quadratic;
};
