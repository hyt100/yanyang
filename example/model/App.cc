#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 0.0f, 1.5f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyPerspectiveCamera::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pShader_ = yyShader::create("../shader/basic.vert", "../shader/basic.frag");

    pModel_ = yyModel::create("../assets/shield/shield.ply");
    std::vector<yyTexture::Ptr> pTextures;
    pTextures.push_back(yyTexture::create("../assets/shield/shield_diffuse.png", yyTextureType_DIFFUSE, false));
    pTextures.push_back(yyTexture::create("../assets/shield/shield_spec.png", yyTextureType_SPECULAR, false));
    for (auto &pMesh: pModel_->getMeshs()) {
        pMesh->setTextures(pTextures);
    }
}

void App::update()
{
}

void App::draw()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    for (auto &pMesh: pModel_->getMeshs()) {
        pShader_->begin();
        pShader_->setInt("useVertexColor", false);
        pMesh->draw(*pCamera_, *pShader_, false);
        pShader_->end();
    }
    yyShaderCheckError();
}