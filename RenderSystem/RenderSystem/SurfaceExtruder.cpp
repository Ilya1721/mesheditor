#include "SurfaceExtruder.h"

#include "MeshCore/Vertex.h"
#include "MeshCore/Face.h"
#include "MeshCore/Mesh.h"
#include "GeometryCore/Plane.h"

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
            mScene->getRenderer().setHighlightedFaces({});
            mScene->getParentWindow()->enableSceneMovement(true);
            mScene->getRenderer().renderVerticesNormals(mScene->getRootObject().getMesh().getVertices());
        }
    }

    void SurfaceExtruder::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
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
        mScene->updateRenderBuffer();
    }

    void SurfaceExtruder::onMouseClick()
    {
        if (mEnabled)
        {
            mSurfaceIntersection = mScene->getClosestIntersection(unProjectCursorPos());
            mScene->getRenderer().setHighlightedFaces(mSurfaceIntersection.surfaceIndices);
        }
    }
}