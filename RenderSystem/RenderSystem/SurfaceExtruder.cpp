#include "SurfaceExtruder.h"

#include "MeshCore/Vertex.h"
#include "MeshCore/Face.h"
#include "MeshCore/Mesh.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Numeric.h"

#include "Constants.h"
#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    SurfaceExtruder::SurfaceExtruder(Scene* scene) :
        Operation(scene),
        mSurfaceMovementEnabled(false)
    {}

    void SurfaceExtruder::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        if (!mEnabled || mSurfaceIntersection.surfaceIndices.empty() || !mSurfaceMovementEnabled)
        {
            return;
        }

        auto& surface = mSurfaceIntersection.intersectedSurface;
        auto window = mScene->getParentWindow();
        auto startCursorPosInWorld = window->unProject(startCursorPos);
        auto endCursorPosInWorld = window->unProject(endCursorPos);
        auto surfaceNormal = glm::normalize(surface.normal);
        auto cursorMovement = glm::normalize(endCursorPosInWorld - startCursorPosInWorld);
        auto surfaceMovement = surfaceNormal * glm::dot(surfaceNormal, cursorMovement);
        std::unordered_set<MeshCore::UniqueVertex*> changedVertices;

        for (auto& face : surface.faces)
        {
            face->move(surfaceMovement, changedVertices);
        }

        auto& parentMesh = (*surface.faces.begin())->parentMesh;
        parentMesh->updateVertices(changedVertices);
        mScene->updateRenderBuffer();
    }

    void SurfaceExtruder::onMouseClick()
    {
        if (mEnabled && mScene->getParentWindow()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            mSurfaceIntersection = mScene->getClosestIntersection();
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
            mScene->enableCameraMovement(false);
        }
        else
        {
            mScene->enableCameraMovement(true);
            toggleSurfaceMovement();
        }
    }

    void SurfaceExtruder::highlightIntersectedSurface()
    {
        if (mSurfaceMovementEnabled)
        {
            mScene->getRenderer().setHighlightedFaces(mSurfaceIntersection.surfaceIndices);
        }
    }

    void SurfaceExtruder::toggleSurfaceMovement(bool isSurfaceIntersected)
    {
        if (mSurfaceMovementEnabled)
        {
            mScene->getRootObject().calculateBBox();
            mScene->getRenderer().setHighlightedFaces({});
        }

        mSurfaceMovementEnabled = isSurfaceIntersected && !mSurfaceMovementEnabled;
    }
}