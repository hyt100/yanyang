#include "App.h"

void App::setup()
{
    glm::vec3 cameraPosition(0.0f, 0.0f, 1.5f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    pCamera_ = yyCameraPerspective::create(yyFrambuffWidth, yyFrambuffHeight, 45.0f, 0.1f, 100.0f, cameraPosition, cameraTarget);

    pShader_ = yyShader::create("../shader/blinnphong.vert", "../shader/blinnphong.frag");
    pShaderSkybox_ = yyShader::create("../shader/skybox.vert", "../shader/skybox.frag");

    std::vector<std::string> images = {
        std::string("../assets/skybox3/right.jpg"),
        std::string("../assets/skybox3/left.jpg"),
        std::string("../assets/skybox3/top.jpg"),
        std::string("../assets/skybox3/bottom.jpg"),
        std::string("../assets/skybox3/front.jpg"),
        std::string("../assets/skybox3/back.jpg")
    };
    pSkybox_ = yyMeshSkybox::create(images);

    pModel_ = yyModel::create("../assets/shield/shield.ply"); //shield.ply中没有带贴图，因此需要加载完模型后补上纹理
    std::vector<yyTexture::Ptr> pTextures;
    pTextures.push_back(yyTexture::create("../assets/shield/shield_diffuse.png", yyTextureType_DIFFUSE, false));
    pTextures.push_back(yyTexture::create("../assets/shield/shield_spec.png", yyTextureType_SPECULAR, false));
    std::cout << "model mesh num: " << pModel_->getMeshs().size() << std::endl;
    std::vector<yyMesh::Ptr> &pMeshs = pModel_->getMeshs();
    pMeshs[0]->setTextures(pTextures);

    ambientLight_.color = glm::vec3(0.5f, 0.5f, 0.5f);
    ambientLight_.intensity = 1.0f;

    pointLight_.position = glm::vec3(0.0f, 1.0f, 1.5f);
    pointLight_.color = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight_.intensity = 1.0f;
    pointLight_.constant = 1.0f;
    pointLight_.linear = 0.09f;
    pointLight_.quadratic = 0.032f;
}

void App::update(float deltaTime)
{
}

void App::draw()
{
    // 渲染model
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    pShader_->begin();
    pShader_->setInt("useVertexColor", false);
    // light properties
    pShader_->setVec3("light.position", yyTransformPoint(pCamera_->viewMat_, pointLight_.position));
    pShader_->setVec3("light.color_ambient", ambientLight_.color * ambientLight_.intensity);
    pShader_->setVec3("light.color", pointLight_.color * pointLight_.intensity);
    pShader_->setFloat("light.constant", pointLight_.constant);
    pShader_->setFloat("light.linear", pointLight_.linear);
    pShader_->setFloat("light.quadratic", pointLight_.quadratic);
    // material properties
    pShader_->setFloat("material.shininess", 128.0f);
    // draw
    pModel_->draw(pCamera_, pShader_, false);
    pShader_->end();

    // 渲染skybox
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    pShaderSkybox_->begin();
    pSkybox_->setTranslation(pCamera_->position_);
    pSkybox_->draw(pCamera_, pShaderSkybox_, false);
    pShaderSkybox_->end();
    glDepthFunc(GL_LESS); // set depth function back to default
    
    yyShaderCheckError();
}