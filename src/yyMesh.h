#pragma once
#include "yyUtil.h"

class yyMesh
{
public:
    yyMesh();
    ~yyMesh();

    void addVertex(const std::vector<float> &vertexs)  { vertexs_.insert(vertexs_.end(), vertexs.begin(), vertexs.end()); }
    void addNormal(const std::vector<float> &normals)  { normals_.insert(normals_.end(), normals.begin(), normals.end()); }
    void addColor(const std::vector<float> &colors)    { colors_.insert(colors_.end(), colors.begin(), colors.end()); }
    void addUV(const std::vector<float> &uvs)          { uvs_.insert(uvs_.end(), uvs.begin(), uvs.end()); }
    void addIndices(const std::vector<unsigned int> &indices) { indices_.insert(indices_.end(), indices.begin(), indices.end()); }

    void clear();
    void bulid();
    void draw(bool wireframeMode = false);

private:
    std::vector<float> vertexs_;
    std::vector<float> normals_;
    std::vector<float> colors_;
    std::vector<float> uvs_;
    std::vector<unsigned int> indices_;
    unsigned int vao_;
    unsigned int ebo_;
    std::vector<unsigned int> vbos_;
};
