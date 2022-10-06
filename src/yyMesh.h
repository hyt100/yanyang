#pragma once
#include "yyUtil.h"
#include "yyTexture.h"
#include "yyShader.h"
#include "yyCamera.h"

#define yyMAX_BONE_INFLUENCE 4

class yyMesh
{
public:
    using Ptr = std::shared_ptr<yyMesh>;
    static Ptr create()
    {
        // return std::make_shared<yyMesh>();
        return Ptr(new yyMesh());
    }
    ~yyMesh();

    void setAttrVertex(const std::vector<glm::vec3> &vertexs)        { vertexs_ = vertexs; }
    void setAttrNormal(const std::vector<glm::vec3> &normals)        { normals_ = normals; }
    void setAttrColor(const std::vector<glm::vec4> &colors)          { colors_ = colors; }
    void setAttrTexCoord(const std::vector<glm::vec2> &texCoords)   { texCoords_ = texCoords; }
    void setAttrTangent(const std::vector<glm::vec3> &tangents)     { tangents_ = tangents; }
    void setAttrBitangent(const std::vector<glm::vec3> &bitangents) { bitangents_ = bitangents; }
    void setAttrIndice(const std::vector<unsigned int> &indices)    { indices_ = indices; }
    void setAttrBoneID(const std::vector<int> &boneIDs)
    {
        if (boneIDs.size() == yyMAX_BONE_INFLUENCE)
            boneIDs_ = boneIDs;
    }
    void setAttrBoneWeight(const std::vector<float> &boneWeights)
    {
        if (boneWeights.size() == yyMAX_BONE_INFLUENCE)
            boneWeights_ = boneWeights;
    }
    void setTextures(std::vector<yyTexture::Ptr> &pTextures) { pTextures_ = pTextures; }

    void bulid();

    void setScale(const glm::vec3 &scale);
    void setRotation(const glm::quat &quat);
    void setRotation(float degree, const glm::vec3 &axis);
    void setTranslation(const glm::vec3 &position);
    void setModelMatrix(const glm::mat4 &mat);

    void draw(const yyCamera &camera, yyShader &pShader, bool wireframeMode = false);

private:
    yyMesh();
private:
    void updateMatrix();

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

    bool needUpdateMat_;
    glm::mat4 modelMat_;
    glm::mat4 modelMatScale_;
    glm::mat4 modelMatRotate_;
    glm::mat4 modelMatTranslate_;
};
