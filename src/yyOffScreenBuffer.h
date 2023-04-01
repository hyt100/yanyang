#pragma once
#include "yyUtil.h"
#include "yyTexture.h"

// TODO: 离屏缓冲支持MSAA
class yyOffScreenBuffer
{
public:
    using Ptr = std::shared_ptr<yyOffScreenBuffer>;
    static Ptr create(unsigned int width, unsigned int height, unsigned int channels = 3, bool hdrColor = false)
    {
        return Ptr(new yyOffScreenBuffer(width, height, channels, hdrColor));
    }
    ~yyOffScreenBuffer();

    yyTexture::Ptr& getTexture()       { return pTexture_; }
    void bind();
    void unbind();

private:
    yyOffScreenBuffer(unsigned int width, unsigned int height, unsigned int channels, bool hdrColor);
private:
    unsigned int width_;
    unsigned int height_;
    unsigned int channels_;
    bool hdrColor_;
    unsigned int framebuffer_;
    // unsigned int textureId_; // color attachment texture
    yyTexture::Ptr pTexture_;   // color attachment texture
    unsigned int rbo_;          // renderbuffer object for depth and stencil attachment
    GLint backupFramebuffer_;
    GLint backupViewport_[4];
};