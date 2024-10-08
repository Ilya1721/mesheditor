#include "SurfaceHighlighter.h"

#include "MeshCore/Intersection.h"

#include "Window.h"

namespace RenderSystem
{
    SurfaceHighlighter::SurfaceHighlighter(Window* window) :
        Operation(window)
    {}

    void SurfaceHighlighter::onMouseMove([[maybe_unused]] const Point2D& startCursorPos, [[maybe_unused]] const Point2D& endCursorPos)
    {
        if (mEnabled)
        {
            auto intersectionData = mWindow->getClosestIntersection();
            mWindow->setHighlightedFacesData({ intersectionData.intersection.surfaceIndices, intersectionData.intersectedObject });
        }
    }

    void SurfaceHighlighter::onKeyPressed(int key)
    {
        if (key != GLFW_KEY_H)
        {
            return;
        }

        mEnabled = !mEnabled;

        if (!mEnabled)
        {
            mWindow->setHighlightedFacesData({});
        }
    }
}