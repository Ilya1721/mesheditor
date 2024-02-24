#include "SurfaceExtruder.h"

#include "Scene.h"

namespace RenderSystem
{
    SurfaceExtruder::SurfaceExtruder(Scene* scene) :
        Operation(scene)
    {}

    void SurfaceExtruder::onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
    {
        auto mouseMoveVector = endCursorPos - startCursorPos;
    }

    void SurfaceExtruder::onMouseClick(const glm::vec2& cursorPos)
    {
        mSurfaceIntersection = mScene->getSurfaceIntersection(unProject(cursorPos));
    }
}