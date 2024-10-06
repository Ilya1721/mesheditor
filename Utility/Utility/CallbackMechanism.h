#pragma once

#include <vector>
#include <functional>

namespace Utility
{
    template <typename CallbackFunc>
    class CallbackMechanism
    {
    public:
        void addCallback(const std::function<CallbackFunc>& callback)
        {
            mCallbacks.push_back(callback);
        }

        void addCallbacks(const CallbackMechanism& other)
        {
            mCallbacks.insert(mCallbacks.end(), other.mCallbacks.begin(), other.mCallbacks.end());
        }

        template <typename... CallbackArgs>
        void invokeCallbacks(CallbackArgs... callbackArgs)
        {
            for (auto& callback : mCallbacks)
            {
                callback(callbackArgs...);
            }
        }

    private:
        std::vector<std::function<CallbackFunc>> mCallbacks;
    };
}

