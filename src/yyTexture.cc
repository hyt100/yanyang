#include "yyTexture.h"


yyTexture::yyTexture()
    :texture_(0)
{
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

yyTexture::~yyTexture()
{
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
    }
}

int yyTexture::load(const std::string &filename)
{
    yyImage image;
    image.load(filename, true);

    glBindTexture(GL_TEXTURE_2D, texture_);

    // 加载并生成纹理
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}