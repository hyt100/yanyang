#include "yyMesh.h"

#define YY_ATTR_VERTEX      0
#define YY_ATTR_COLOR       1
#define YY_ATTR_NORMAL      2
#define YY_ATTR_TEXCOORD    3
#define YY_ATTR_TANGENT     4
#define YY_ATTR_BITANGENT   5
#define YY_ATTR_MAX         6

yyMesh::yyMesh()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ebo_);
    vbos_.resize(YY_ATTR_MAX);
    glGenBuffers(vbos_.size(), vbos_.data());
}

yyMesh::~yyMesh()
{
    glDeleteBuffers(vbos_.size(), vbos_.data());
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
}

void yyMesh::bulid()
{
    if (vertexs_.size() == 0 || indices_.size() == 0) {
        throw std::runtime_error("vertexs or indices can't be empty!");
    }

    glBindVertexArray(vao_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexs_.size() * sizeof(glm::vec3), vertexs_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(YY_ATTR_VERTEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(YY_ATTR_VERTEX);
 
    if (colors_.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_COLOR]);
        glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec3), colors_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_COLOR);
    }
    if (normals_.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), normals_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_NORMAL);
    }
    if (texCoords_.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER, texCoords_.size() * sizeof(glm::vec2), texCoords_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_TEXCOORD);
    }
    if (tangents_.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_TANGENT]);
        glBufferData(GL_ARRAY_BUFFER, tangents_.size() * sizeof(glm::vec3), tangents_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_TANGENT, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_TANGENT);
    }
    if (bitangents_.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_BITANGENT]);
        glBufferData(GL_ARRAY_BUFFER, bitangents_.size() * sizeof(glm::vec3), bitangents_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_BITANGENT, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_BITANGENT);
    }

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void yyMesh::draw(yyShader::Ptr &pShader, bool wireframeMode)
{
    // bind appropriate textures
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr   = 0;
    unsigned int ambientNr  = 0;
    for (unsigned int i = 0; i < pTextures_.size(); ++i)
    {
        yyTextureType type = pTextures_[i]->getType();
        std::string name = pTextures_[i]->getName();
        unsigned int textureId = pTextures_[i]->getId();
        std::string samplerName;

        if (type == yyTextureType_DIFFUSE)
            samplerName = name + std::to_string(diffuseNr++);
        else if (type == yyTextureType_SPECULAR)
            samplerName = name + std::to_string(specularNr++);
        else if (type == yyTextureType_NORMAL)
            samplerName = name + std::to_string(normalNr++);
        else if (type == yyTextureType_AMBIENT)
            samplerName = name + std::to_string(ambientNr++);
        else
            samplerName = name;

        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);     // 在绑定之前，激活纹理单元
        // now set the sampler to the correct texture unit
        pShader->setTextureUnit(samplerName, i);  // 告诉着色器的采样器使用哪个纹理单元
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textureId); // 绑定纹理对象到激活的纹理单元 
    }

    if (wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}


