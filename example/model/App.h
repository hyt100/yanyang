#pragma once
#include "yyCore.h"

class App: public yyBaseApp
{
public:
    App() {}
    virtual ~App() {}
    void setup();

    void update();
    void keyprocess(yyEvent event)
    {
        if (pCamera_) {
            pCamera_->keyprocess(event);
        }
    }

    void draw();

private:
    yyPerspectiveCamera::Ptr pCamera_;
    yyShader::Ptr pShader_;
    yyModel::Ptr pModel_;
    yyPointLight pointLight_;
};