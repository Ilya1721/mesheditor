#pragma once

#include <vector>
#include <functional>

namespace Utility
{
    class CallbackMechanism
    {
    public:
        void addCallback(const std::function<void()>& callback);
        void invokeCallbacks();

    private:
        std::vector<std::function<void()>> mCallbacks;
    };
}
