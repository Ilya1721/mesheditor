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

    void SurfaceHighlighter::onMouseMove([[maybe_unused]] const Point2D& startCursorPos, [[maybe_unused]] const Point2D& endCursorPos)
    {
        if (mEnabled)
        {
            mScene->getRenderer().setHighlightedFaces(mScene->getClosestIntersection().surfaceIndices);
        }
    }
}