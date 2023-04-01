#include "yyTexture.h"


yyTexture::yyTexture(const std::string &filename, yyTextureType type, bool flipY)
{
    auto pImage = yyImage::create(filename, flipY);
    filename_ = filename;
    encoding_ = yyImageEncodingLinear;
    textureType_ = type;
    init(pImage);
}

yyTexture::yyTexture(uint8_t *buffer, int bufferSize, yyTextureType type, bool flipY)
{
    auto pImage = yyImage::create(buffer, bufferSize, flipY);
    filename_ = "";
    encoding_ = yyImageEncodingLinear;
    textureType_ = type;
    init(pImage);
}

void yyTexture::init(yyImage::Ptr &pImage)
{
    auto nrComponents = pImage->getChannel();
    auto width = pImage->getWidth();
    auto height = pImage->getHeight();
    auto data = pImage->data();
    
    GLint  internalFormat;
    GLenum format;
    if (nrComponents == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB;
    } else {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }
    GLenum type = GL_UNSIGNED_BYTE;

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    // (第二个参数为纹理指定多级渐远纹理的级别，如果启用mipmap的的话可设置。这里我们填0，也就是基本级别)
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// 加载CubeMap
yyTexture::yyTexture(const std::vector<std::string> &cubemapFilenames)
{
    if (cubemapFilenames.size() != 6) {
        throw std::runtime_error("Failed to load cubemap texture! ");
    }
    filename_ = "";
    encoding_ = yyImageEncodingLinear;
    textureType_ = yyTextureType_CUBEMAP;

    yyImage::Ptr pImages[6];
    for (int i = 0; i < 6; ++i) {
        pImages[i] = yyImage::create(cubemapFilenames[i], false);
    }
    auto nrComponents = pImages[0]->getChannel();
    auto width = pImages[0]->getWidth();
    auto height = pImages[0]->getHeight();
    
    GLint  internalFormat;
    GLenum format;
    if (nrComponents == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB;
    } else {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }
    GLenum type = GL_UNSIGNED_BYTE;

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // R坐标设置了环绕方式，它对应的是纹理的第三个维度（和位置的z一样）
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载顺序为：
    //     GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
    //     GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
    //     GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
    //     GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
    //     GL_TEXTURE_CUBE_MAP_POSITIVE_Z	前
    //     GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	后
    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pImages[i]->data());
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// 创建纹理, 仅分配了内存而没有填充它
yyTexture::yyTexture(unsigned int width, unsigned int height, unsigned int channels, bool hdrColor)
{
    filename_ = "";
    encoding_ = yyImageEncodingLinear;
    textureType_ = yyTextureType_NONE;

    GLenum format;
    GLint  internalFormat;
    if (channels == 3) {
        format = GL_RGB;
        internalFormat = hdrColor ? GL_RGB16F : GL_RGB;
    }
    else {
        format = GL_RGBA;
        internalFormat = hdrColor ? GL_RGBA16F : GL_RGBA;
    }
    
    GLenum type;
    if (hdrColor)
        type = GL_FLOAT;
    else
        type = GL_UNSIGNED_BYTE;
    
    // std::cout << "texture channel: " << channels << std::endl;
    
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

yyTexture::~yyTexture()
{
    if (textureId_ != 0) {
        glDeleteTextures(1, &textureId_);
    }
}

std::string yyTexture::getName()
{
    switch (textureType_) {
        case yyTextureType_DIFFUSE:   return "texture_diffuse";
        case yyTextureType_SPECULAR:  return "texture_specular";
        case yyTextureType_NORMAL:    return "texture_normal";
        case yyTextureType_AMBIENT:   return "texture_ambient";
        case yyTextureType_CUBEMAP:   return "texture_cubemap";
        case yyTextureType_NONE:      return "texture_none";
        default:                      return "texture_unknow";
    }
}