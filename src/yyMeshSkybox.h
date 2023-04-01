#pragma once
#include "yyMesh.h"

class yyMeshSkybox: public yyMesh
{
public:
    using Ptr = std::shared_ptr<yyMeshSkybox>;
    static Ptr create(const std::vector<std::string> &images)
    {
        return Ptr(new yyMeshSkybox(images));
    }
    ~yyMeshSkybox() {}

private:
    yyMeshSkybox(const std::vector<std::string> &images);

private:
    std::vector<yyTexture::Ptr> pTextures_;
};
