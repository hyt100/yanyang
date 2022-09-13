#pragma once
#include "yyUtil.h"

struct yyMaterial {
    glm::vec3 ambient;  // 漫反射贴图可以取代该项
    glm::vec3 diffuse;  // 漫反射贴图可以取代该项
    glm::vec3 specular; // 镜面高光贴图可以取代该项
    float shininess;
};
