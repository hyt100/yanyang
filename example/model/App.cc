#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 30.0f, 30.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyPerspectiveCamera::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pShader_ = yyShader::create("../example/triangle/shader.vert", "../example/triangle/shader.frag");

    pModel_ = yyModel::create("../assets/nanosuit/nanosuit.obj");
}

void App::update()
{
    glm::mat4 vpMat = pCamera_->getViewProjectMat();
    mvpMat_ = vpMat * modelMat_;
}

void App::draw()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (auto &pMesh: pModel_->getMeshs()) {
        pShader_->begin();
        pShader_->setMat4("mvp", mvpMat_);
        pMesh->draw(pShader_);
        pShader_->end();
    }
    yyShaderCheckError();
}