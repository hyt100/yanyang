#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 5.0f, 0.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, -5.0f);
    pCamera_ = yyCameraPerspective::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pTexture_ = yyTexture::create("../assets/lena_512x512.jpg", yyTextureType_DIFFUSE, true);

    pShader_ = yyShader::create("../shader/basic.vert", "../shader/basic.frag");
    pShader_->begin();
    pShader_->setBool("useVertexColor", false);
    pShader_->end();

    pPlane_ = yyMeshPlane::create(20.0f, 20.0f);
    pPlane_->setRotation(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}

void App::update(float deltaTime)
{
}

void App::draw()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pShader_->begin();
    pPlane_->setTextures(pTexture_);
    pPlane_->draw(pCamera_, pShader_, false);
    pShader_->end();

    yyShaderCheckError();
}