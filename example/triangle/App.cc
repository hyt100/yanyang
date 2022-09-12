#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyPerspectiveCamera::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f,  0.5f, 0.0f)
    };
    std::vector<glm::vec4> colors = {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    };
    std::vector<glm::vec2> texCoords = {
        glm::vec2(0.0f, 0.0f), 
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)
    };
    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    pShader_ = yyShader::create("../shader/basic.vert", "../shader/basic.frag");
    pTextures_.push_back(yyTexture::create("../assets/lena_512x512.jpg", yyTextureType_DIFFUSE, true));

    pMesh_ = yyMesh::create();
    pMesh_->addVertex(vertices);
    pMesh_->addIndices(indices);
    pMesh_->addColor(colors);
    pMesh_->addTexCoords(texCoords);
    pMesh_->addTextures(pTextures_);
    pMesh_->bulid();
}

void App::update()
{
}

void App::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pShader_->begin();
    pShader_->setBool("useVertexColor", true);
    pMesh_->draw(*pCamera_, pShader_, false);
    pShader_->end();
    yyShaderCheckError();
}