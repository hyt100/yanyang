#pragma once
#include "yyMesh.h"

class yySkybox
{
public:
    using Ptr = std::shared_ptr<yySkybox>;
    static Ptr create(const std::vector<std::string> &images)
    {
        // return std::make_shared<yySkybox>(images);
        return Ptr(new yySkybox(images));
    }
    ~yySkybox() {}

    void setRotation(float degree, const glm::vec3 &axis) { pMesh_->setRotation(degree, axis); }
    void setRotation(const glm::quat &quat)               { pMesh_->setRotation(quat); }
    void setTranslation(const glm::vec3 &position)        { pMesh_->setTranslation(position); }
    void draw(const yyCamera &camera, yyShader &pShader, bool wireframeMode = false);

private:
    yySkybox(const std::vector<std::string> &images);

private:
    yyMesh::Ptr pMesh_;
    std::vector<yyTexture::Ptr> pTextures_;
};
