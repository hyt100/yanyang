#pragma once
#include "yyUtil.h"
#include "yyTexture.h"
#include "yyShader.h"

#define yyMAX_BONE_INFLUENCE 4

class yyMesh
{
public:
    using Ptr = std::shared_ptr<yyMesh>;
    static Ptr create()
    {
        return std::make_shared<yyMesh>();
    }

    yyMesh();
    ~yyMesh();

    void addVertex(const std::vector<glm::vec3> &vertexs)        { vertexs_ = vertexs; }
    void addNormal(const std::vector<glm::vec3> &normals)        { normals_ = normals; }
    void addColor(const std::vector<glm::vec4> &colors)          { colors_ = colors; }
    void addTexCoords(const std::vector<glm::vec2> &texCoords)   { texCoords_ = texCoords; }
    void addTangents(const std::vector<glm::vec3> &tangents)     { tangents_ = tangents; }
    void addBitangents(const std::vector<glm::vec3> &bitangents) { bitangents_ = bitangents; }
    void addIndices(const std::vector<unsigned int> &indices)    { indices_ = indices; }
    void addBoneIDs(const std::vector<int> &boneIDs)
    {
        if (boneIDs.size() == yyMAX_BONE_INFLUENCE)
            boneIDs_ = boneIDs;
    }
    void addBoneWeights(const std::vector<float> &boneWeights)
    {
        if (boneWeights.size() == yyMAX_BONE_INFLUENCE)
            boneWeights_ = boneWeights;
    }
    void addTextures(std::vector<yyTexture::Ptr> &pTextures) { pTextures_ = pTextures; }

    void bulid();
    void draw(yyShader::Ptr &pShader, bool wireframeMode = false);

private:
    std::vector<glm::vec3> vertexs_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec4> colors_;  // vertex color
    std::vector<glm::vec2> texCoords_;
    std::vector<glm::vec3> tangents_;
    std::vector<glm::vec3> bitangents_;
    std::vector<unsigned int> indices_;
    std::vector<int> boneIDs_;        // bone indexes which will influence this vertex
    std::vector<float> boneWeights_;  // weights from each bone
    unsigned int vao_;
    unsigned int ebo_;
    std::vector<unsigned int> vbos_;
    std::vector<yyTexture::Ptr> pTextures_;
};
