#include "CallbackMechanism.h"

namespace Utility
{
    void CallbackMechanism::addCallback(const std::function<void()>& callback)
    {
        mCallbacks.push_back(callback);
    }

    void CallbackMechanism::invokeCallbacks()
    {
        for (auto& callback : mCallbacks)
        {
            callback();
        }
    }
}
