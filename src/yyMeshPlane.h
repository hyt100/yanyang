#pragma once
#include "yyMesh.h"

class yyMeshPlane: public yyMesh
{
public:
    using Ptr = std::shared_ptr<yyMeshPlane>;
    static Ptr create(float width, float height, const glm::vec4 &color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
    {
        return Ptr(new yyMeshPlane(width, height, color));
    }
    static Ptr createQuad() // for pitcture
    {
        return create(2.0f, 2.0f); // the range of [-1,1] for NDC
    }
    virtual ~yyMeshPlane() = default;

private:
    yyMeshPlane(float width, float height, const glm::vec4 &color);
};