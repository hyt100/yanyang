#pragma once
#include "yyCore.h"

class App: public yyBaseApp
{
public:
    App() {}
    virtual ~App() {}
    void setup();

    void update();

    void draw();

private:
    yyShader shader_;
    yyTexture texture_;
    yyMesh mesh_;
};