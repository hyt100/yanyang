#pragma once
#include "yyCore.h"

class App: public yyBaseApp
{
public:
    App() {}
    virtual ~App() {}
    void setup();
    void keyprocess(yyEvent event)
    {
        if (pCamera_) {
            pCamera_->keyprocess(event);
        }
    }
    void update(float deltaTime);
    void draw();

private:
    yyCameraPerspective::Ptr pCamera_;
    yyShader::Ptr pShader_;
    yyShader::Ptr pShaderSkybox_;
    yyModel::Ptr pModel_;
    yyMeshSkybox::Ptr pSkybox_;
    yyAmbientLight ambientLight_;
    yyPointLight pointLight_;
};