#pragma once
#include "yyMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class yyModel
{
public:
    using Ptr = std::shared_ptr<yyModel>;
    static Ptr create(const std::string &filename)
    {
        // return std::make_shared<yyModel>(filename);
        return Ptr(new yyModel(filename));
    }

    ~yyModel() {}
    std::vector<yyMesh::Ptr> &getMeshs() { return pMeshs_; }

    void setScale(const glm::vec3 &scale)                 { for (auto &pMesh: pMeshs_) pMesh->setScale(scale); }
    void setRotation(float degree, const glm::vec3 &axis) { for (auto &pMesh: pMeshs_) pMesh->setRotation(degree, axis); }
    void setRotation(const glm::quat &quat)               { for (auto &pMesh: pMeshs_) pMesh->setRotation(quat); }
    void setTranslation(const glm::vec3 &position)        { for (auto &pMesh: pMeshs_) pMesh->setTranslation(position); }
    void setModelMatrix(const glm::mat4 &mat)             { for (auto &pMesh: pMeshs_) pMesh->setModelMatrix(mat); }

private:
    yyModel(const std::string &filename);
private:
    void processNode(const aiScene *scene, aiNode *node);
    yyMesh::Ptr processMesh(const aiScene *scene, aiMesh *mesh);
    void loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::vector<yyTexture::Ptr> &out);
    yyTextureType convertTextureType(aiTextureType type);

    std::vector<yyMesh::Ptr> pMeshs_;
    std::string directory_;
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<yyTexture::Ptr> pTexturesLoaded_;
};
