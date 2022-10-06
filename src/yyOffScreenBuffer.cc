#include "yyOffScreenBuffer.h"

yyOffScreenBuffer::yyOffScreenBuffer(unsigned int width, unsigned int height, unsigned int channels)
{
    width_ = width;
    height_ = height;
    channels_ = channels;

    // create a framebuffer
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    // create a color attachment texture (对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。)
    // GLenum format;
    // if (channels == 1)
    //     format = GL_RED;
    // else if (channels == 3)
    //     format = GL_RGB;
    // else
    //     format = GL_RGBA;
    // glGenTextures(1, &textureId_);
    // glBindTexture(GL_TEXTURE_2D, textureId_);
    // glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0); // 附加这个纹理到framebuffer
    pTexture_ = yyTexture::create(width, height, channels);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture_->getId(), 0); // 附加这个纹理到framebuffer

    // create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_); // 附加这个渲染缓冲对象到framebuffer

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

yyOffScreenBuffer::~yyOffScreenBuffer()
{
    glDeleteFramebuffers(1, &framebuffer_);
    // glDeleteTextures(1, &textureId_);
    glDeleteRenderbuffers(1, &rbo_);
}

void yyOffScreenBuffer::bind()
{
    // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backupFramebuffer_);
    // glGetIntegerv(GL_VIEWPORT, backupViewport_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    // glViewport(0, 0, width_, height_);
}

void yyOffScreenBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glBindFramebuffer(GL_FRAMEBUFFER, backupFramebuffer_);
    // glViewport(backupViewport_[0], backupViewport_[1], backupViewport_[2], backupViewport_[3]);
}
