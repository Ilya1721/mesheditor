#include "SurfaceHighlighter.h"

#include "MeshCore/Intersection.h"

#include "Scene.h"
#include "Window.h"
#include "GlobalRenderState.h"

namespace RenderSystem
{
    SurfaceHighlighter::SurfaceHighlighter(Scene* scene) :
        Operation(scene)
    {}

    void SurfaceHighlighter::onMouseMove([[maybe_unused]] const Point2D& startCursorPos, [[maybe_unused]] const Point2D& endCursorPos)
    {
        if (mEnabled)
        {
            GlobalRenderState::setHighlightedFaces(mScene->getClosestIntersection().surfaceIndices);
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
            GlobalRenderState::setHighlightedFaces({});
        }
    }
}