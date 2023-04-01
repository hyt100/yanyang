#pragma once
#include "yyUtil.h"

class yyBaseApp
{
public:
    yyBaseApp() {}
    virtual ~yyBaseApp() {}
    virtual void setup() {}
    virtual void keyprocess(yyEvent event) {}
    virtual void update() 
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
};