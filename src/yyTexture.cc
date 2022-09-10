#include "yyTexture.h"


yyTexture::yyTexture(const std::string &filename, yyTextureType type, bool flipY, yyImageEncoding encoding)
{
    auto pImage = yyImage::create(filename, flipY);
    filename_ = filename;
    encoding_ = encoding;
    textureType_ = type;
    init(pImage);
}

yyTexture::yyTexture(uint8_t *buffer, int bufferSize, yyTextureType type, bool flipY, yyImageEncoding encoding)
{
    auto pImage = yyImage::create(buffer, bufferSize, flipY);
    filename_ = "";
    encoding_ = encoding;
    textureType_ = type;
    init(pImage);
}

void yyTexture::init(yyImage::Ptr &pImage)
{
    auto nrComponents = pImage->getChannel();
    auto width = pImage->getWidth();
    auto height = pImage->getHeight();
    auto data = pImage->data();
    
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    // (第二个参数为纹理指定多级渐远纹理的级别，如果启用mipmap的的话可设置。这里我们填0，也就是基本级别)
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

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
        default:                      return "texture_unknow";
    }
}