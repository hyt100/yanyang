#include "yyPostProcess.h"

yyPostProcess::yyPostProcess(unsigned int width, unsigned int height)
{
    pingPongBuffer_[0] = yyOffScreenBuffer::create(width, height, 3);
    pingPongBuffer_[1] = yyOffScreenBuffer::create(width, height, 3);
    isRenderToTexture_ = false;
}

void yyPostProcess::beginPass(bool renderToTexture)
{
    isRenderToTexture_ = renderToTexture;

    if (isRenderToTexture_) {
        pingPongBuffer_[0]->bind();
    }
}

void yyPostProcess::endPass()
{
    if (isRenderToTexture_) {
        pingPongBuffer_[0]->unbind();
        std::swap(pingPongBuffer_[0], pingPongBuffer_[1]);  // pingPongBuffer_[1]存放渲染结果
    }
}
