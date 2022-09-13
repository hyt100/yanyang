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

    needUpdateMat_ = false;
    modelMat_ = glm::mat4(1.0f);
    modelMatScale_ = glm::mat4(1.0f);
    modelMatRotate_ = glm::mat4(1.0f);
    modelMatTranslate_ = glm::mat4(1.0f);
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
    auto vertexsSize = vertexs_.size();

    glBindVertexArray(vao_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexs_.size() * sizeof(glm::vec3), vertexs_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(YY_ATTR_VERTEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(YY_ATTR_VERTEX);
 
    if (colors_.size()) {
        if (colors_.size() != vertexsSize) {
            std::cout << "warning: color size abnormal" << std::endl;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_COLOR]);
        glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec4), colors_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_COLOR);
    }
    if (normals_.size()) {
        if (normals_.size() != vertexsSize) {
            std::cout << "warning: normals size abnormal" << std::endl;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), normals_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_NORMAL);
    }
    if (texCoords_.size()) {
        if (texCoords_.size() != vertexsSize) {
            std::cout << "warning: texCoords size abnormal" << std::endl;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER, texCoords_.size() * sizeof(glm::vec2), texCoords_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_TEXCOORD);
    }
    if (tangents_.size()) {
        if (tangents_.size() != vertexsSize) {
            std::cout << "warning: tangents size abnormal" << std::endl;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbos_[YY_ATTR_TANGENT]);
        glBufferData(GL_ARRAY_BUFFER, tangents_.size() * sizeof(glm::vec3), tangents_.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(YY_ATTR_TANGENT, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(YY_ATTR_TANGENT);
    }
    if (bitangents_.size()) {
        if (bitangents_.size() != vertexsSize) {
            std::cout << "warning: bitangents size abnormal" << std::endl;
        }
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

void yyMesh::setScale(const glm::vec3 &scale)
{
    needUpdateMat_ = true;
    modelMatScale_ = glm::scale(glm::mat4(1.0f), scale);
}

void yyMesh::setRotation(float degree, const glm::vec3 &axis)
{
    needUpdateMat_ = true;
    modelMatRotate_ = glm::rotate(glm::mat4(1.0f), glm::radians(degree), axis);
}

void yyMesh::setTranslation(const glm::vec3 &position)
{
    needUpdateMat_ = true;
    modelMatTranslate_ = glm::translate(glm::mat4(1.0f), position);
}

void yyMesh::setModelMatrix(const glm::mat4 &mat)
{
    needUpdateMat_ = false;
    modelMat_ = mat;
}

void yyMesh::updateMatrix()
{
    if (needUpdateMat_) {
        modelMat_ = modelMatTranslate_ * modelMatRotate_ * modelMatScale_;
    }
}

void yyMesh::draw(const yyCamera &camera, yyShader &shader, bool wireframeMode)
{
    // calculate MVP
    updateMatrix();
    glm::mat4 mvpMat = camera.viewProjectionMat_ * modelMat_;
    glm::mat4 mvMat = camera.viewMat_ * modelMat_;
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(mvMat)));  // remove translation component
    shader.setMat4("mMat", modelMat_);
    shader.setMat4("mvMat", mvMat);
    shader.setMat4("mvpMat", mvpMat);
    shader.setMat3("normalMat", normalMat);

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
        glActiveTexture(GL_TEXTURE0 + i);        // 在绑定之前，激活纹理单元
        // now set the sampler to the correct texture unit
        shader.setTextureUnit(samplerName, i);   // 告诉着色器的采样器使用哪个纹理单元
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
