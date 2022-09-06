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

inline float yyRadiansToDegree(float value)
{
    return 180 * value / yyPI;
}
