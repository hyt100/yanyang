#pragma once
#include "yyUtil.h"

class yyBaseApp
{
public:
    yyBaseApp() {}
    virtual ~yyBaseApp() {}
    virtual void setup() {}                    // 初始化
    virtual void keyprocess(yyEvent event) {}  // 按键处理
    virtual void update(float deltaTime) {}    // 此函数执行不耗时操作
    virtual void draw()                        // 此函数执行drawcall，可能很耗时
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
};