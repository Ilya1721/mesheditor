#include "SurfaceHighlighter.h"

#include "MeshCore/Intersection.h"

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
            mScene->getRenderer().setHighlightedFaces({});
        }
    }

    void SurfaceHighlighter::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        if (mEnabled)
        {
            auto cursorPos = mScene->getParentWindow()->getCursorPos();
            auto surfaceIntersection = mScene->getClosestIntersection(unProject(cursorPos));
            mScene->getRenderer().setHighlightedFaces(surfaceIntersection.surfaceIndices);
        }
    }
}