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

        if (!mEnabled)
        {
            mScene->highlightFaces({});
        }
    }

    void SurfaceExtruder::onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
    {
        if (!mEnabled || mSurfaceIntersection.surfaceIndices.empty())
        {
            return;
        }

        auto startPosOnTargetPlane = mScene->getParentWindow()->unProjectToCameraTargetPlane(startCursorPos);
        auto endPosOnTargetPlane = mScene->getParentWindow()->unProjectToCameraTargetPlane(endCursorPos);
        auto unitMouseMoveVector = glm::normalize(endPosOnTargetPlane - startPosOnTargetPlane);

        glm::vec3 surfaceNormal = mScene->getViewMatrix() * glm::vec4(mSurfaceIntersection.intersectedSurface.normal, 1.0f);
        auto surfaceMoveVector = surfaceNormal * glm::dot(surfaceNormal, unitMouseMoveVector) * SURFACE_EXTRUCTION_SPEED_KOEF;
        std::unordered_set<MeshCore::Vertex*> changedVertices;
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