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
    void update();

private:
    yyCameraPerspective::Ptr pCamera_;
    yyShader::Ptr pShaderEffective0_;
    yyShader::Ptr pShaderEffective1_;
    yyTexture::Ptr pTexture_;
    yyMeshPlane::Ptr pQuad_;
    yyPostProcess::Ptr pPostProcess_;
};