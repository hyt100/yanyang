#pragma once
#include <string>
#include <stdint.h>


class yyImage
{
public:
    yyImage();
    ~yyImage();

    int load(const std::string &filename, bool flipY = false);

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
