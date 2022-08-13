#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class yyNoncopyable
{
public:
    yyNoncopyable(const yyNoncopyable&) = delete;
    void operator=(const yyNoncopyable&) = delete;
protected:
    yyNoncopyable() = default;
    ~yyNoncopyable() = default;
};
