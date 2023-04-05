#pragma once
#include "yyMesh.h"
#include "yyBone.h"
#include "yyAssimpHelper.h"

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
    void draw(const yyCamera::Ptr &pCamera, yyShader::Ptr &pShader, bool wireframeMode = false)
    {
        for (auto &pMesh: pMeshs_)
            pMesh->draw(pCamera, pShader, wireframeMode);
    }

private:
    yyModel(const std::string &filename);
private:
    void processNode(const aiScene *scene, aiNode *node);
    yyMesh::Ptr processMesh(const aiScene *scene, aiMesh *mesh);
    void loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::vector<yyTexture::Ptr> &out);
    void loadBoneWeightForVertices(const aiScene *scene, aiMesh *mesh, std::vector<glm::ivec4> &boneIDs, std::vector<glm::vec4> &boneWeights);

    std::vector<yyMesh::Ptr> pMeshs_;
    std::string rootDirectory_;
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::map<std::string, yyTexture::Ptr> texturesMap_;
    std::map<std::string, yyBoneInfo> boneInfoMap_;
    unsigned int boneCounter_ = 0;
};
