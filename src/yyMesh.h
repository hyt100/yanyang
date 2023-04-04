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
    virtual ~yyMesh();

    virtual void setAttrVertex(const std::vector<glm::vec3> &vertexs)         { vertexs_ = vertexs;         }
    virtual void setAttrNormal(const std::vector<glm::vec3> &normals)         { normals_ = normals;         }
    virtual void setAttrColor(const std::vector<glm::vec4> &colors)           { colors_ = colors;           }
    virtual void setAttrTexCoord(const std::vector<glm::vec2> &texCoords)     { texCoords_ = texCoords;     }
    virtual void setAttrTangent(const std::vector<glm::vec3> &tangents)       { tangents_ = tangents;       }
    virtual void setAttrBitangent(const std::vector<glm::vec3> &bitangents)   { bitangents_ = bitangents;   }
    virtual void setAttrIndice(const std::vector<unsigned int> &indices)      { indices_ = indices;         }
    virtual void setAttrBoneID(const std::vector<glm::ivec4> &boneIDs)        { boneIDs_ = boneIDs;         }
    virtual void setAttrBoneWeight(const std::vector<glm::vec4> &boneWeights) { boneWeights_ = boneWeights; }
    virtual void setTextures(std::vector<yyTexture::Ptr> &pTextures)          { pTextures_ = pTextures;     }
    virtual void setTextures(yyTexture::Ptr &pTextures)
    {
        pTextures_.clear(); 
        pTextures_.emplace_back(pTextures); // single texture
    }

    virtual void build();

    virtual void setScale(const glm::vec3 &scale);
    virtual void setRotation(const glm::quat &quat);
    virtual void setRotation(float degree, const glm::vec3 &axis);
    virtual void setTranslation(const glm::vec3 &position);
    virtual void setModelMatrix(const glm::mat4 &mat);

    virtual void draw(const yyCamera::Ptr &pCamera, yyShader::Ptr &pShader, bool wireframeMode = false);

protected:
    yyMesh();
    void updateMatrix();

    std::vector<glm::vec3> vertexs_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec4> colors_;  // vertex color
    std::vector<glm::vec2> texCoords_;
    std::vector<glm::vec3> tangents_;
    std::vector<glm::vec3> bitangents_;
    std::vector<unsigned int> indices_;
    std::vector<glm::ivec4> boneIDs_;      // bone indexes which will influence this vertex (每个顶点允许四个bone)
    std::vector<glm::vec4>  boneWeights_;  // weights from each bone
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
