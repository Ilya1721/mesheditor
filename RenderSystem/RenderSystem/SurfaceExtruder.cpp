#include "SurfaceExtruder.h"

#include "MeshCore/Vertex.h"
#include "MeshCore/Face.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/Object3D.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Numeric.h"

#include "Constants.h"
#include "Window.h"
#include "GlobalRenderState.h"
#include "Camera.h"

namespace
{
    using namespace RenderSystem;

    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
    Window* gWindow = &Window::getInstance();
    Camera* gCamera = &Camera::getInstance();
}

namespace RenderSystem
{
    SurfaceExtruder::SurfaceExtruder() :
        mSurfaceMovementEnabled(false)
    {}

    void SurfaceExtruder::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        if (!mEnabled || mSurfaceIntersection.surfaceIndices.empty() || !mSurfaceMovementEnabled)
        {
            return;
        }

        auto& surface = mSurfaceIntersection.surface;
        auto startCursorPosInWorld = gWindow->unProject(startCursorPos);
        auto endCursorPosInWorld = gWindow->unProject(endCursorPos);
        auto surfaceNormal = glm::normalize(surface.normal);
        auto cursorMovement = glm::normalize(endCursorPosInWorld - startCursorPosInWorld);
        auto surfaceMovement = surfaceNormal * glm::dot(surfaceNormal, cursorMovement);
        std::unordered_set<MeshCore::UniqueVertex*> changedVertices;

        for (auto& face : surface.faces)
        {
            face->move(surfaceMovement, changedVertices);
        }

        (*surface.faces.begin())->parentMesh->getParentObject()->onMeshUpdated(changedVertices);
    }

    void SurfaceExtruder::onMouseClick()
    {
        if (mEnabled && gWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            mSurfaceIntersection = gGlobalRenderState->getRootObject()->findIntersection(gWindow->castCursorRay(), true);
            toggleSurfaceMovement(!mSurfaceIntersection.surfaceIndices.empty());
            highlightIntersectedSurface();
        }
    }

    void SurfaceExtruder::onKeyPressed(int key)
    {
        if (key != GLFW_KEY_E)
        {
            return;
        }

        mEnabled = !mEnabled;

        if (mEnabled)
        {
            gCamera->enableMovement(false);
        }
        else
        {
            gCamera->enableMovement(true);
            toggleSurfaceMovement();
        }
    }

    void SurfaceExtruder::highlightIntersectedSurface() const
    {
        if (mSurfaceMovementEnabled)
        {
            gGlobalRenderState->setHighlightedFacesData({ mSurfaceIntersection.surfaceIndices, mSurfaceIntersection.surface.getParentObject() });
        }
    }

    void SurfaceExtruder::toggleSurfaceMovement(bool isSurfaceIntersected)
    {
        if (mSurfaceMovementEnabled)
        {
            gGlobalRenderState->setHighlightedFacesData({});
        }

        mSurfaceMovementEnabled = isSurfaceIntersected && !mSurfaceMovementEnabled;
    }
}