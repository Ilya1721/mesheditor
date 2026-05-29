#include "BRepFactory.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "BRepCurve.h"
#include "BRepEdge.h"
#include "Constants.h"
#include "TransformUtils.h"

namespace MeshCore
{
  NURBSCurve3D createUnitXYBRepCircle()
  {
    std::vector<glm::vec4> controlPoints = {{1, 0, 0, 1},  {1, 1, 0, 1},  {0, 1, 0, 1},
                                            {-1, 1, 0, 1}, {-1, 0, 0, 1}, {-1, -1, 0, 1},
                                            {0, -1, 0, 1}, {1, -1, 0, 1}, {1, 0, 0, 1}};
    float diagonalWeight = std::sqrt(2.0f) / 2.0f;
    std::vector<float> weights = {1, diagonalWeight, 1, diagonalWeight, 1, diagonalWeight,
                                  1, diagonalWeight, 1};
    std::vector<float> boundaries = {0,    0,     0,     0.25f, 0.25f, 0.5f,
                                     0.5f, 0.75f, 0.75f, 1,     1,     1};
    int smoothnessLevel = 2;

    return NURBSCurve3D(smoothnessLevel, boundaries, controlPoints, weights);
  }

  NURBSSurface createUnitXYBRepPlane()
  {
    NURBSSurface surface;
    float halfWidth = 0.5f;
    float halfHeight = 0.5f;
    std::vector<std::vector<glm::vec4>> controlPoints = {
      std::vector<glm::vec4> {
        glm::vec4(-halfWidth, -halfHeight, 0.0f, 1.0f),
        glm::vec4(-halfWidth, halfHeight, 0.0f, 1.0f)
      },
      std::vector<glm::vec4> {
        glm::vec4(halfWidth, -halfHeight, 0.0f, 1.0f),
        glm::vec4(halfWidth, halfHeight, 0.0f, 1.0f)
      }
    };
    surface.setControlPoints(controlPoints);
    std::vector<std::vector<float>> weights = {
      std::vector<float> {1.0f, 1.0f}, std::vector<float> {1.0f, 1.0f}
    };
    surface.setWeights(weights);
    std::vector<float> boundariesU {0, 0, 1, 1};
    std::vector<float> boundariesV {0, 0, 1, 1};
    surface.setBoundaries(boundariesU, boundariesV);
    surface.setSmoothnessLevels(1, 1);

    return surface;
  }

  NURBSSurface createUnitBRepCylinder()
  {
    const auto U = 9;
    const auto V = 2;
    const auto diagonalWeight = std::sqrt(2.0f) / 2.0f;

    struct CircleControlPoint
    {
      float x;
      float y;
      float weight;
    };

    const CircleControlPoint circleControlPoints[U] = {
      {1.0f, 0.0f, 1.0f},  {1.0f, 1.0f, diagonalWeight},
      {0.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, diagonalWeight},
      {-1.0f, 0.0f, 1.0f}, {-1.0f, -1.0f, diagonalWeight},
      {0.0f, -1.0f, 1.0f}, {1.0f, -1.0f, diagonalWeight},
      {1.0f, 0.0f, 1.0f}
    };

    std::vector<std::vector<glm::vec4>> controlPoints(U);
    std::vector<std::vector<float>> weights(U);

    for (size_t idx = 0; idx < U; ++idx)
    {
      controlPoints[idx].resize(V);
      weights[idx].resize(V);
      const auto& circleControlPoint = circleControlPoints[idx];

      controlPoints[idx][0] = {circleControlPoint.x, circleControlPoint.y, 0.0f, 1.0f};
      weights[idx][0] = circleControlPoint.weight;
      controlPoints[idx][1] = {circleControlPoint.x, circleControlPoint.y, 1.0f, 1.0f};
      weights[idx][1] = circleControlPoint.weight;
    }

    std::vector<float> boundariesU = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4};
    std::vector<float> boundariesV = {0, 0, 1, 1};

    NURBSSurface surface;
    surface.setBoundaries(boundariesU, boundariesV);
    surface.setControlPoints(controlPoints);
    surface.setWeights(weights);
    surface.setSmoothnessLevels(2, 1);

    return surface;
  }

  BRepModel createBRepCylinder(
    const glm::vec3& origin, const glm::vec3& normal, float radius, float height
  )
  {
    glm::vec3 centerBottom = origin;
    glm::vec3 centerTop = origin + glm::normalize(normal) * height;

    auto bottomCircleTransform = getUnitXYCircleTransform(centerBottom, normal, radius);
    auto bottomCircle = createUnitXYBRepCircle();
    bottomCircle.applyTransform(bottomCircleTransform);
    auto topCircleTransform = getUnitXYCircleTransform(centerTop, normal, radius);
    auto topCircle = createUnitXYBRepCircle();
    topCircle.applyTransform(topCircleTransform);

    auto bottomEdge = std::make_unique<BRepEdge>();
    auto topEdge = std::make_unique<BRepEdge>();
    bottomEdge->curve = std::make_unique<NURBSCurve3D>(std::move(bottomCircle));
    topEdge->curve = std::make_unique<NURBSCurve3D>(std::move(topCircle));

    auto sideFace = std::make_unique<BRepFace>();
    auto bottomFace = std::make_unique<BRepFace>();
    auto topFace = std::make_unique<BRepFace>();

    auto cylinderSurface = std::make_unique<NURBSSurface>(createUnitBRepCylinder());
    auto cylinderTransform = getUnitCylinderTransform(origin, normal, radius, height);
    cylinderSurface->applyTransform(cylinderTransform);
    sideFace->surface = cylinderSurface.get();
    sideFace->uvMapper = std::make_unique<AffineFaceUVMapper>();

    auto planeSize = 2.0f * radius;
    auto bottomPlane = std::make_unique<NURBSSurface>(createUnitXYBRepPlane());
    auto bottomPlaneTransform =
      getUnitXYPlaneTransform(centerBottom, normal, planeSize, planeSize);
    bottomPlane->applyTransform(bottomPlaneTransform);
    bottomFace->surface = bottomPlane.get();
    bottomFace->uvMapper = std::make_unique<AffineFaceUVMapper>();

    auto topPlane = std::make_unique<NURBSSurface>(createUnitXYBRepPlane());
    auto topPlaneTransform =
      getUnitXYPlaneTransform(centerTop, normal, planeSize, planeSize);
    topPlane->applyTransform(topPlaneTransform);
    topFace->surface = topPlane.get();
    topFace->uvMapper = std::make_unique<AffineFaceUVMapper>();

    sideFace->outerLoop = std::make_unique<BRepLoop>();
    topFace->outerLoop = std::make_unique<BRepLoop>();
    bottomFace->outerLoop = std::make_unique<BRepLoop>();

    auto sideTopHalfEdge = std::make_unique<BRepHalfEdge>();
    sideTopHalfEdge->edge = topEdge.get();
    sideTopHalfEdge->face = sideFace.get();
    sideTopHalfEdge->isReversed = true;

    auto sideBottomHalfEdge = std::make_unique<BRepHalfEdge>();
    sideBottomHalfEdge->edge = bottomEdge.get();
    sideBottomHalfEdge->face = sideFace.get();
    sideBottomHalfEdge->isReversed = false;

    sideTopHalfEdge->next = sideBottomHalfEdge.get();
    sideTopHalfEdge->prev = sideBottomHalfEdge.get();
    sideBottomHalfEdge->next = sideTopHalfEdge.get();
    sideBottomHalfEdge->prev = sideTopHalfEdge.get();

    auto topHalfEdge = std::make_unique<BRepHalfEdge>();
    topHalfEdge->edge = topEdge.get();
    topHalfEdge->face = topFace.get();
    topHalfEdge->isReversed = false;
    topHalfEdge->next = topHalfEdge.get();
    topHalfEdge->prev = topHalfEdge.get();

    auto bottomHalfEdge = std::make_unique<BRepHalfEdge>();
    bottomHalfEdge->edge = bottomEdge.get();
    bottomHalfEdge->face = bottomFace.get();
    bottomHalfEdge->isReversed = true;
    bottomHalfEdge->next = bottomHalfEdge.get();
    bottomHalfEdge->prev = bottomHalfEdge.get();

    sideTopHalfEdge->pCurve = std::make_unique<CylinderCurve2D>(height);
    sideBottomHalfEdge->pCurve = std::make_unique<CylinderCurve2D>(0);
    topHalfEdge->pCurve = std::make_unique<CircleCurve2D>(radius);
    bottomHalfEdge->pCurve = std::make_unique<CircleCurve2D>(radius);

    sideFace->outerLoop->start = sideTopHalfEdge.get();
    topFace->outerLoop->start = topHalfEdge.get();
    bottomFace->outerLoop->start = bottomHalfEdge.get();

    BRepModel model;
    model.edges.push_back(std::move(bottomEdge));
    model.edges.push_back(std::move(topEdge));
    model.halfEdges.push_back(std::move(sideTopHalfEdge));
    model.halfEdges.push_back(std::move(sideBottomHalfEdge));
    model.halfEdges.push_back(std::move(topHalfEdge));
    model.halfEdges.push_back(std::move(bottomHalfEdge));
    model.surfaces.push_back(std::move(bottomPlane));
    model.surfaces.push_back(std::move(topPlane));
    model.surfaces.push_back(std::move(cylinderSurface));
    model.faces.push_back(std::move(sideFace));
    model.faces.push_back(std::move(bottomFace));
    model.faces.push_back(std::move(topFace));

    return model;
  }
}
