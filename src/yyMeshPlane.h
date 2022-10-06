#pragma once
#include "yyMesh.h"

class yyMeshPlane
{
public:
    using Ptr = std::shared_ptr<yyMeshPlane>;
    static Ptr create(float width, float height, const glm::vec4 &color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
    {
        return Ptr(new yyMeshPlane(width, height, color));
    }
    ~yyMeshPlane() = default;

    void setTexture(yyTexture::Ptr &pTexture)
    {
        std::vector<yyTexture::Ptr> pTextures_ = { pTexture };
        pMesh_->setTextures(pTextures_);
    }
    void setScale(const glm::vec3 &scale)                 { pMesh_->setScale(scale); }
    void setRotation(float degree, const glm::vec3 &axis) { pMesh_->setRotation(degree, axis); }
    void setRotation(const glm::quat &quat)               { pMesh_->setRotation(quat); }
    void setTranslation(const glm::vec3 &position)        { pMesh_->setTranslation(position); }

    void draw(const yyCamera &camera, yyShader &pShader, bool wireframeMode = false) { pMesh_->draw(camera, pShader, wireframeMode); }

private:
    yyMeshPlane(float width, float height, const glm::vec4 &color);

private:
    yyMesh::Ptr pMesh_;
};