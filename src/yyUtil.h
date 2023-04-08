#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>  //for glm::to_string
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <map>

#define yyPI             3.14159265359
#define yyEPSINON        0.00001
#define yyFloatEQ(a, b)  (std::fabs((a) - (b)) < yyEPSINON)  //Equal
#define yyFloatNE(a, b)  (!yyFloatEQ((a), (b)))         //Not Equal
#define yyFloatGT(a, b)  ((a) - (b) > yyEPSINON)        //Greater Than
#define yyFloatLT(a, b)  ((a) - (b) < -yyEPSINON)       //Less Than
#define yyFloatGE(a, b)  ((a) - (b) > -yyEPSINON)       //Greater Than or Equal
#define yyFloatLE(a, b)  ((a) - (b) < yyEPSINON)        //Less Than or Equal


enum yyEvent {
    YYEVENT_NONE = 0,
    YYEVENT_UP,
    YYEVENT_DOWN,
    YYEVENT_LEFT,
    YYEVENT_RIGHT,
    YYEVENT_ZOOMIN,
    YYEVENT_ZOOMOUT
};

class yyNoncopyable
{
public:
    yyNoncopyable(const yyNoncopyable&) = delete;
    void operator=(const yyNoncopyable&) = delete;
protected:
    yyNoncopyable() = default;
    ~yyNoncopyable() = default;
};

inline float yyRad2Deg(float value)
{
    return 180.0f * value / yyPI;
}

inline float yyDeg2Rad(float value)
{
    return yyPI * value / 180.0f;
}

inline glm::vec3 yyTransformPoint(const glm::mat4 &mat, const glm::vec3 &p)
{
    glm::vec4 p_homo = glm::vec4(p.x, p.y, p.z, 1.0f);
    return glm::vec3(mat * p_homo);
}

inline glm::vec3 yyTransformDirection(const glm::mat4 &mat, const glm::vec3 &p)
{
    glm::vec4 p_homo = glm::vec4(p.x, p.y, p.z, 0.0f);
    return glm::vec3(mat * p_homo);
}