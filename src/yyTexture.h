#pragma once
#include "yyUtil.h"
#include "yyImage.h"

class yyTexture
{
public:
    yyTexture();
    ~yyTexture();

    int load(const std::string &filename);
    unsigned int getTexture() { return texture_; }

private:
    unsigned int texture_;
};