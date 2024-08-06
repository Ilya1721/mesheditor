#include "GlobalExtraPrimitives.h"

#include "GeometryCore/Line.h"
#include "MeshCore/RootRenderDataStorage.h"

#include "Constants.h"
#include "Scene.h"

namespace RenderSystem
{
	std::vector<RenderPrimitive> GlobalExtraPrimitives::gExtraPrimitives;
	constexpr Material FLOOR_MATERIAL = PEARL_MATERIAL;
}

namespace RenderSystem
{
	const std::vector<RenderPrimitive>& GlobalExtraPrimitives::getExtraPrimitives()
	{
		return gExtraPrimitives;
	}

	void GlobalExtraPrimitives::addSceneFloor()
	{
		auto originY = -Scene::getRootObject().getBBox().getHeight() * FLOOR_BBOX_HEIGHT_COEF;
		addPlane(Point3D(0.0f, originY, 0.0f), Vector3D(0.0f, -1.0f, 0.0f), FAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE, FLOOR_MATERIAL);
	}

	void GlobalExtraPrimitives::add(const RenderPrimitive& primitive)
	{
		gExtraPrimitives.push_back(primitive);
		MeshCore::RootRenderDataStorage::appendExtraRenderData(primitive.renderData);
	}

	void GlobalExtraPrimitives::addPlane(const Point3D& origin, const Vector3D& normal, float width, float height, const Material& material)
	{
		Plane plane{ origin, normal };
		auto planePrimitive = RenderPrimitive::createPrimitive(plane, width, height, material);
		add(planePrimitive);
	}

	void GlobalExtraPrimitives::addLine(const Point3D& start, const Point3D& end, const Material& material)
	{
		Line line{ start, end };
		auto linePrimitive = RenderPrimitive::createPrimitive(line, true, material);
		add(linePrimitive);
	}

	void GlobalExtraPrimitives::addGlobalAxes(float length)
	{
		addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(length, 0.0f, 0.0f), BLUE_MATERIAL);
		addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, length, 0.0f), RED_MATERIAL);
		addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, length), GREEN_MATERIAL);
	}

	void GlobalExtraPrimitives::addVerticesNormals(const std::vector<MeshCore::Vertex>& vertices)
	{
		for (const auto& vertex : vertices)
		{
			addLine(vertex.pos, vertex.pos + vertex.normal * 10.0f, GREEN_MATERIAL);
		}
	}
}