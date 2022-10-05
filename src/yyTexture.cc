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

// 加载CubeMap
yyTexture::yyTexture(const std::vector<std::string> &cubemapFilenames, yyImageEncoding encoding)
{
    if (cubemapFilenames.size() != 6) {
        throw std::runtime_error("Failed to load cubemap texture! ");
    }
    filename_ = "";
    encoding_ = encoding;
    textureType_ = yyTextureType_CUBEMAP;

    yyImage::Ptr pImages[6];
    for (int i = 0; i < 6; ++i) {
        pImages[i] = yyImage::create(cubemapFilenames[i], false);
    }
    auto nrComponents = pImages[0]->getChannel();
    auto width = pImages[0]->getWidth();
    auto height = pImages[0]->getHeight();
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

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
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pImages[i]->data());
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
        default:                      return "texture_unknow";
    }
}