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
    glm::mat4 modelMat_ = glm::mat4(1.0f);
    glm::mat4 mvpMat_ = glm::mat4(1.0f);
    yyShader::Ptr pShader_;
    yyModel::Ptr pModel_;
};