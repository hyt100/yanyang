#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyPerspectiveCamera::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pPostProcess_ = yyPostProcess::create(yyFrambuffWidth, yyFrambuffHeight);
    pTexture_ = yyTexture::create("../assets/lena_512x512.jpg", yyTextureType_DIFFUSE, true);

    pShader_ = yyShader::create("../shader/basic.vert", "../shader/basic.frag");

    pShaderEffective_ = yyShader::create("../shader/effective.vert", "../shader/effective_inversion.frag");

    pPlane_ = yyMeshPlane::create(1.0f, 1.0f);
}

void App::update()
{
}

void App::draw()
{
    pPostProcess_->beginPass(true);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pShader_->begin();
    pPlane_->setTexture(pTexture_);
    pShader_->setBool("useVertexColor", false);
    pPlane_->draw(*pCamera_, *pShader_, false);
    pShader_->end();
    pPostProcess_->endPass();

    pPostProcess_->beginPass(false);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pShaderEffective_->begin();
    auto pTextureEffective = pPostProcess_->getTexture();
    pPlane_->setTexture(pTextureEffective);
    pShaderEffective_->setInt("width", yyFrambuffWidth);
    pShaderEffective_->setInt("height", yyFrambuffHeight);
    pPlane_->draw(*pCamera_, *pShaderEffective_, false);
    pShaderEffective_->end();
    pPostProcess_->endPass();

    yyShaderCheckError();
}