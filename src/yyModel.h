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
        return std::make_shared<yyModel>(filename);
    }

    yyModel(const std::string &filename);
    ~yyModel() {}
    std::vector<yyMesh::Ptr> &getMeshs() { return pMeshs_; }

private:
    void processNode(aiNode *node, const aiScene *scene);
    yyMesh::Ptr processMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::vector<yyTexture::Ptr> &out);
    yyTextureType convertTextureType(aiTextureType type);

    std::vector<yyMesh::Ptr> pMeshs_;
    std::string directory_;
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<yyTexture::Ptr> pTexturesLoaded_;
};
