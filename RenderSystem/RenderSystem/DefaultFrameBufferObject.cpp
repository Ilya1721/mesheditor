#include "DefaultFrameBufferObject.h"

namespace RenderSystem
{
  DefaultFrameBufferObject::DefaultFrameBufferObject() : FrameBufferObject()
  {
    mFBO = 0;
    mDepthRBO = 0;
  }
}
