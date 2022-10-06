#include "yyMeshPlane.h"

yyMeshPlane::yyMeshPlane(float width, float height, const glm::vec4 &color)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    std::vector<glm::vec3> vertices = {
        glm::vec3(-halfW, +halfH, 0.0f),
        glm::vec3(-halfW, -halfH, 0.0f),
        glm::vec3(+halfW, -halfH, 0.0f),
        glm::vec3(+halfW, +halfH, 0.0f),
    };
    std::vector<glm::vec3> normals = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    std::vector<glm::vec4> colors = {
        color, color, color, color
    };
    std::vector<glm::vec2> texCoords = { // bottom-left coordinate system
        glm::vec2(0.0f, 1.0f), 
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    pMesh_ = yyMesh::create();
    pMesh_->setAttrVertex(vertices);
    pMesh_->setAttrNormal(normals);
    pMesh_->setAttrIndice(indices);
    pMesh_->setAttrColor(colors);
    pMesh_->setAttrTexCoord(texCoords);
    pMesh_->bulid();
}