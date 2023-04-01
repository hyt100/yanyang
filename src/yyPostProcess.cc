#include "yyPostProcess.h"

yyPostProcess::yyPostProcess(unsigned int width, unsigned int height)
{
    pingPongBuffer_[0] = yyOffScreenBuffer::create(width, height, 3, false);
    pingPongBuffer_[1] = yyOffScreenBuffer::create(width, height, 3, false);
}

void yyPostProcess::beginPass()
{
    pingPongBuffer_[0]->bind();
}

void yyPostProcess::endPass()
{
    pingPongBuffer_[0]->unbind();
    std::swap(pingPongBuffer_[0], pingPongBuffer_[1]);  // pingPongBuffer_[1]存放渲染结果
}
