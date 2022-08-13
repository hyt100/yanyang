#pragma once
#include "yyUtil.h"

class yyBaseApp
{
public:
    yyBaseApp() {}
    virtual ~yyBaseApp() {}
    virtual void setup() {}
    virtual void update() {}
    virtual void draw() 
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};