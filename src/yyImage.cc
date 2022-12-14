#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "yyImage.h"
#include "yyUtil.h"

yyImage::yyImage(const std::string &filename, bool flipY)
{
    stbi_set_flip_vertically_on_load(flipY);
    data_ = stbi_load(filename.c_str(), &width_, &height_, &channels_, 0);
    if (!data_) {
        throw std::runtime_error("Failed to load image");
    }
    std::cout << "load image ok (" << width_ << "x" << height_ << "  " << channels_ << ")" << std::endl;
}

yyImage::yyImage(uint8_t *buffer, int bufferSize, bool flipY)
{
    stbi_set_flip_vertically_on_load(flipY);
    data_ = stbi_load_from_memory(buffer, bufferSize, &width_, &height_, &channels_, 0);
    if (!data_) {
        throw std::runtime_error("Failed to load image");
    }
    std::cout << "load image ok (" << width_ << "x" << height_ << "  " << channels_ << ")" << std::endl;
}

yyImage::~yyImage()
{
    if (data_) {
        stbi_image_free(data_);
    }
}
