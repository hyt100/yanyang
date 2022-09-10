#pragma once
#include <string>
#include <stdint.h>
#include "yyUtil.h"

class yyImage
{
public:
    using Ptr = std::shared_ptr<yyImage>;
    static Ptr create(const std::string &filename, bool flipY = false)
    {
        return std::make_shared<yyImage>(filename, flipY);
    }
    static Ptr create(uint8_t *buffer, int bufferSize, bool flipY = false)
    {
        return std::make_shared<yyImage>(buffer, bufferSize, flipY);
    }

    yyImage(const std::string &filename, bool flipY);
    yyImage(uint8_t *buffer, int bufferSize, bool flipY);
    ~yyImage();

    int getWidth()    { return width_; }
    int getHeight()   { return height_; }
    int getChannel()  { return channels_; }
    uint8_t* data()   { return data_; }

private:
    uint8_t *data_;
    int width_;
    int height_;
    int channels_;
};
