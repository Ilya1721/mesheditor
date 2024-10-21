#include "Bindable.h"

namespace RenderSystem
{
    void Bindable::invokeAction(const std::function<void()>& action)
    {
        bind();
        action();
        unbind();
    }
}
