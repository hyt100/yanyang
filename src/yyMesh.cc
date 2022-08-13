#include "yyMesh.h"

#define YY_ATTR_LOCATION_VERTEX  0
#define YY_ATTR_LOCATION_COLOR   1
#define YY_ATTR_LOCATION_NORMAL  2
#define YY_ATTR_LOCATION_UV      3

yyMesh::yyMesh()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ebo_);
}

yyMesh::~yyMesh()
{
    for (auto vbo : vbos_) {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo_ != 0) {
        glDeleteBuffers(1, &ebo_);
    }
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
    }
}

void yyMesh::clear()
{
    for (auto vbo : vbos_) {
        glDeleteBuffers(1, &vbo);
    }
    vertexs_.clear();
    indices_.clear();
    colors_.clear();
    normals_.clear();
    uvs_.clear();
}

void yyMesh::bulid()
{
    if (vertexs_.size() == 0 || indices_.size() == 0) {
        throw std::runtime_error("vertexs or indices can't be empty!");
    }

    glBindVertexArray(vao_);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexs_.size() * sizeof(float), vertexs_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(YY_ATTR_LOCATION_VERTEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(YY_ATTR_LOCATION_VERTEX);
    vbos_.push_back(vbo);
    
    if (colors_.size()) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), colors_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_LOCATION_COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_LOCATION_COLOR);
        vbos_.push_back(vbo);
    }
    if (normals_.size()) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(float), normals_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_LOCATION_NORMAL);
        vbos_.push_back(vbo);
    }
    if (uvs_.size()) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(float), uvs_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_LOCATION_UV);
        vbos_.push_back(vbo);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void yyMesh::draw(bool wireframeMode)
{
    if (wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
