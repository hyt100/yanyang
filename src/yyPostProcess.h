#pragma once
#include "yyUtil.h"
#include "yyOffScreenBuffer.h"

// enum yyPPPassType
// {
//     YY_PP_PASS_FIRST = 0,  // 第一个pass，渲染整个场景到纹理中
//     YY_PP_PASS_EFFECTIVE,  // 特效处理pass，对纹理进行处理，结果存储到另一个纹理中
//     YY_PP_PASS_LAST,       // 最后一个pass，将结果渲染到默认framebuffer去显示
//     YY_PP_PASS_MAX
// };

class yyPostProcess
{
public:
    using Ptr = std::shared_ptr<yyPostProcess>;
    static Ptr create(unsigned int width, unsigned int height)
    {
        return Ptr(new yyPostProcess(width, height));
    }
    ~yyPostProcess() = default;
    void beginPass(bool renderToTexture);
    void endPass();
    yyTexture::Ptr getTexture() { return pingPongBuffer_[1]->getTexture(); }

private:
    yyPostProcess(unsigned int width, unsigned int height);

private:
    yyOffScreenBuffer::Ptr pingPongBuffer_[2];
    bool isRenderToTexture_;
};
