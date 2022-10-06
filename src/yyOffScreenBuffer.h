#pragma once
#include "yyUtil.h"

class yyOffScreenBuffer
{
public:
    using Ptr = std::shared_ptr<yyOffScreenBuffer>;
    static Ptr create(unsigned int width, unsigned int height, unsigned int channels = 3)
    {
        return Ptr(new yyOffScreenBuffer(width, height, channels));
    }
    ~yyOffScreenBuffer() = default;

    unsigned int getTextureId()    { return textureId_; }
    void bind();
    void unbind();

private:
    yyOffScreenBuffer(unsigned int width, unsigned int height, unsigned int channels);
private:
    unsigned int width_;
    unsigned int height_;
    unsigned int channels_;
    unsigned int framebuffer_;
    unsigned int textureId_; // color attachment texture
    unsigned int rbo_;       // renderbuffer object for depth and stencil attachment
    GLint backupFramebuffer_;
    GLint backupViewport_[4];
}；