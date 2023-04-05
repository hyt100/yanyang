#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyCameraPerspective::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pTexture_ = yyTexture::create("../assets/lena_512x512.jpg", yyTextureType_DIFFUSE, true);

    pShaderEffective0_ = yyShader::create("../shader/effective.vert", "../shader/effective_blur.frag");
    pShaderEffective1_ = yyShader::create("../shader/effective.vert", "../shader/effective_inversion.frag");

    pQuad_ = yyMeshPlane::createQuad();

    pPostProcess_ = yyPostProcess::create(yyFrambuffWidth, yyFrambuffHeight);
}

void App::update(float deltaTime)
{
}

void App::draw()
{
    // 特效0: 模糊多次
    bool isFirst = true;
    for (int i = 0; i < 300; ++i) {
        pPostProcess_->beginPass();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        pShaderEffective0_->begin();
        if (isFirst) {
            isFirst = false;
            pQuad_->setTextures(pTexture_);
        } else {
            pQuad_->setTextures(pPostProcess_->getTexture());
        }
        pQuad_->draw(pCamera_, pShaderEffective0_, false);
        pShaderEffective0_->end();
        pPostProcess_->endPass();
    }

    // 特效1：反色
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pShaderEffective1_->begin();
    pQuad_->setTextures(pPostProcess_->getTexture());
    pQuad_->draw(pCamera_, pShaderEffective1_, false);
    pShaderEffective1_->end();

    yyShaderCheckError();
}