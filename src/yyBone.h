#pragma once
#include "yyUtil.h"

struct yyBoneInfo {
    unsigned int id;
    glm::mat4 offsetMatrix;

    yyBoneInfo(): id(0), offsetMatrix(1.0f)
    {}
};