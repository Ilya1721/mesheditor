#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
    class Scene;

    class Operation
    {
    public:
        Operation(Scene* scene);
        virtual ~Operation() = default;

        void toggle();

        virtual void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) = 0;
        virtual void onMouseClick(const glm::vec2& cursorPos) = 0;

    protected:
        glm::vec3 unProject(const glm::vec2& cursorPos) const;

    protected:
        bool mEnabled = false;
        Scene* mScene = nullptr;
    };
}
