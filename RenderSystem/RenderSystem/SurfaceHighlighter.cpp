#include "SurfaceHighlighter.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    SurfaceHighlighter::SurfaceHighlighter(Scene* scene) :
        Operation(scene)
    {}

    void SurfaceHighlighter::toggle()
    {
        mEnabled = !mEnabled;

        if (!mEnabled) 
        {
            mScene->highlightFaces({});
        }
    }

    void SurfaceHighlighter::onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
    {
        if (mEnabled)
        {
            auto cursorPos = mScene->getParentWindow()->getCursorPos();
            auto surfaceIntersection = mScene->getClosestIntersection(unProject(cursorPos));
            mScene->highlightFaces(surfaceIntersection.surfaceIndices);
        }
    }
}