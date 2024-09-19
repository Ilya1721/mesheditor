#include "SurfaceHighlighter.h"

#include "MeshCore/Intersection.h"
#include "MeshCore/Object3D.h"

#include "Window.h"
#include "GlobalRenderState.h"

namespace
{
    using namespace RenderSystem;

    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
    Window* gWindow = &Window::getInstance();
}

namespace RenderSystem
{
    void SurfaceHighlighter::onMouseMove([[maybe_unused]] const Point2D& startCursorPos, [[maybe_unused]] const Point2D& endCursorPos)
    {
        if (mEnabled)
        {
            auto intersection = gGlobalRenderState->getRootObject()->findIntersection(gWindow->castCursorRay(), true);
            gGlobalRenderState->setHighlightedFacesData({ intersection.surfaceIndices, intersection.surface.getParentObject() });
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
            gGlobalRenderState->setHighlightedFacesData({});
        }
    }
}