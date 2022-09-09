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
        if (camera_) {
            camera_->keyprocess(event);
        }
    }

    void draw();

private:
    yyPerspectiveCamera::Ptr camera_;
    glm::mat4 modelMat_ = glm::mat4(1.0f);
    glm::mat4 mvpMat_ = glm::mat4(1.0f);
    yyShader::Ptr pShader_;
    std::vector<yyTexture::Ptr> pTextures_;
    yyMesh::Ptr pMesh_;
};