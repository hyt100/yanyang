#pragma once
#include "yyUtil.h"
#include "yyImage.h"

enum yyImageEncoding
{
    yyImageEncodingLinear = 0,
    yyImageEncodingSRGB
};

enum yyTextureType
{
    yyTextureType_DIFFUSE = 0,
    yyTextureType_SPECULAR,
    yyTextureType_NORMAL,
    yyTextureType_AMBIENT
};

class yyTexture
{
public:
    using Ptr = std::shared_ptr<yyTexture>;
    static Ptr create(const std::string &filename, yyTextureType type, bool flipY, yyImageEncoding encoding = yyImageEncodingLinear)
    {
        return std::make_shared<yyTexture>(filename, type, flipY, encoding);
    }
    static Ptr create(uint8_t *buffer, int bufferSize, yyTextureType type, bool flipY, yyImageEncoding encoding = yyImageEncodingLinear)
    {
        return std::make_shared<yyTexture>(buffer, bufferSize, type, flipY, encoding);
    }

    yyTexture(const std::string &filename, yyTextureType type, bool flipY, yyImageEncoding encoding);
    yyTexture(uint8_t *buffer, int bufferSize, yyTextureType type, bool flipY, yyImageEncoding encoding);
    ~yyTexture();

    unsigned int getId()    { return textureId_; }
    yyTextureType getType() { return textureType_; }
    std::string getName();

private:
    void init(yyImage::Ptr &pImage);
    std::string filename_;
    yyTextureType textureType_;
    unsigned int textureId_;
    yyImageEncoding encoding_;
};