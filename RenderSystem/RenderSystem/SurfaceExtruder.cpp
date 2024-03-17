#include "SurfaceExtruder.h"

#include "Constants.h"
#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    SurfaceExtruder::SurfaceExtruder(Scene* scene) :
        Operation(scene)
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
            mScene->highlightFaces({});
            mScene->getParentWindow()->enableSceneMovement(true);
        }
    }

    void SurfaceExtruder::onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
    {
        if (!mEnabled || mSurfaceIntersection.surfaceIndices.empty())
        {
            return;
        }

        auto window = mScene->getParentWindow();
        auto perpendicularPlane = mSurfaceIntersection.intersectedSurface.getPerpendicularPlane();
        auto startPosOnPlane = perpendicularPlane.projectPoint(window->pointOnScreenToPointInWorldSpace(startCursorPos, 1.0f));
        auto endPosOnPlane = perpendicularPlane.projectPoint(window->pointOnScreenToPointInWorldSpace(endCursorPos, 1.0f));
        auto unitMouseMoveVector = glm::normalize(endPosOnPlane - startPosOnPlane);

        auto unitSurfaceNormal = glm::normalize(mSurfaceIntersection.intersectedSurface.normal);
        auto surfaceMoveVector = unitSurfaceNormal * glm::dot(unitSurfaceNormal, unitMouseMoveVector) * SURFACE_EXTRUCTION_SPEED_KOEF;
        std::unordered_set<MeshCore::UniqueVertex*> changedVertices;
        auto& surfaceFaces = mSurfaceIntersection.intersectedSurface.faces;
        
        for (auto& face : surfaceFaces)
        {
            face->move(surfaceMoveVector, changedVertices);
        }

        auto& parentMesh = (*surfaceFaces.begin())->parentMesh;
        parentMesh->updateVertices(changedVertices);
        mScene->updateRenderData();
    }

    void SurfaceExtruder::onMouseClick(const glm::vec2& cursorPos)
    {
        if (mEnabled)
        {
            mSurfaceIntersection = mScene->getClosestIntersection(unProject(cursorPos));
            mScene->highlightFaces(mSurfaceIntersection.surfaceIndices);
        }
    }
}