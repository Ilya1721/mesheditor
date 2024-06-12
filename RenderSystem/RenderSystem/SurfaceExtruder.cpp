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

    void SurfaceExtruder::toggle()
    {
        mEnabled = !mEnabled;

        if (mEnabled)
        {
            mScene->getParentWindow()->enableSceneMovement(false);
        }
        else
        {
            mScene->getParentWindow()->enableSceneMovement(true);
            toggleSurfaceMovement();
        }
    }

    void SurfaceExtruder::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        if (!mEnabled || mSurfaceIntersection.surfaceIndices.empty() || !mSurfaceMovementEnabled)
        {
            return;
        }

        auto& surface = mSurfaceIntersection.intersectedSurface;
        auto window = mScene->getParentWindow();
        auto startCursorPosInWorld = window->pointOnScreenToPointInWorldSpace(startCursorPos, 1.0f);
        auto endCursorPosInWorld = window->pointOnScreenToPointInWorldSpace(endCursorPos, 1.0f);
        auto surfaceNormal = glm::normalize(surface.normal);
        auto cursorMovement = glm::normalize(endCursorPosInWorld - startCursorPosInWorld);
        auto surfaceMovement = surfaceNormal * glm::dot(surfaceNormal, cursorMovement);
        std::unordered_set<MeshCore::UniqueVertex*> changedVertices;

        if (isEqual(surfaceMovement, glm::vec3(0.0f, 0.0f, 0.0f)))
        {
            return;
        }

        //
        mScene->getRenderer().renderLine(mSurfaceIntersection.point, mSurfaceIntersection.point + cursorMovement * 100.0f, GREEN_MATERIAL, true);
        mScene->getRenderer().renderLine(mSurfaceIntersection.point, mSurfaceIntersection.point + surfaceNormal * 100.0f, RED_MATERIAL, true);
        //

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
        if (!mEnabled)
        {
            return;
        }

        mSurfaceIntersection = mScene->getClosestIntersection(unProjectCursorPos());
        toggleSurfaceMovement(!mSurfaceIntersection.surfaceIndices.empty());
        highlightIntersectedSurface();
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