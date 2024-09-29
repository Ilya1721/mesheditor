#include "RenderData.h"

#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Typedefs.h"
#include "GeometryCore/Numeric.h"

#include "MeshCore/Vertex.h"
#include "MeshCore/Constants.h"

namespace
{
	using namespace RenderSystem;

	RenderData generateRenderData(const std::vector<Vertex>& vertices, const glm::mat4& transform)
	{
		RenderData renderData;
		for (const auto& vertex : vertices)
		{
			renderData.append(transform * vertex);
		}

		return renderData;
	}
}

namespace RenderSystem
{
	using namespace GeometryCore;

	void RenderData::append(const RenderData& other)
	{
		mCompactData.insert(std::end(mCompactData), std::cbegin(other.mCompactData), std::cend(other.mCompactData));
	}

	void RenderData::append(const Vertex& vertex)
	{
		for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
		{
			mCompactData.emplace_back(vertex.pos[coordIdx]);
		}
		for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
		{
			mCompactData.emplace_back(vertex.normal[coordIdx]);
		}
	}

	void RenderData::updateVertex(const OriginalVertexData& vertexData, int startVertexOffset)
	{
		for (unsigned int compactDataIdx = 6 * (startVertexOffset + vertexData.index), coordIdx = 0; coordIdx < 3; ++compactDataIdx, ++coordIdx)
		{
			mCompactData[compactDataIdx] = vertexData.vertex->pos[coordIdx];
			mCompactData[compactDataIdx + 3] = vertexData.vertex->normal[coordIdx];
		}
	}

	const std::vector<float>& RenderData::getCompactData() const
	{
		return mCompactData;
	}

	int RenderData::getVertexCount() const
	{
		return mCompactData.size() / 6;
	}

	RenderData RenderData::createRenderData(const Ray& ray, float length)
	{
		RenderData renderData;
		renderData.append({ ray.origin, ray.direction });
		renderData.append({ ray.origin + ray.direction * length, ray.direction });
		
		return renderData;
	}

	RenderData RenderData::createRenderData(const Line& line, bool withArrowHead)
	{
		auto distance = glm::distance(line.start, line.end);
		auto defaultLineDir = glm::vec3(0.0f, distance, 0.0f);
		auto lineDir = line.end - line.start;
		auto crossProduct = glm::cross(defaultLineDir, lineDir);
		if (isEqual(crossProduct, glm::vec3(0.0f, 0.0f, 0.0f)) && glm::dot(defaultLineDir, lineDir) < 0.0f)
		{
			distance = -distance;
		}

		std::vector<Vertex> vertices;
		Line defaultLine{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, distance, 0.0f) };
		vertices.emplace_back(defaultLine.start, glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));

		if (withArrowHead) 
		{
			auto tangent = glm::tan(glm::radians(ARROW_HEAD_ANGLE));
			auto x = tangent * distance * ARROW_HEAD_LENGTH_COEF;
			auto y = defaultLine.end.y - (x / tangent);
			vertices.emplace_back(defaultLine.end, Vector3D(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(Point3D(-x, y, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(defaultLine.end, Vector3D(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(Point3D(x, y, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		}

		return generateRenderData(vertices, line.getTransformToSelf(defaultLine));
	}

	RenderData RenderData::createRenderData(const Plane& plane, float width, float length)
	{
		std::vector<Vertex> vertices;
		auto halfWidth = width * 0.5f;
		auto halfLength = length * 0.5f;
		vertices.emplace_back(Point3D(halfWidth, halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(Point3D(-halfWidth, halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(Point3D(-halfWidth, -halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(Point3D(-halfWidth, -halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(Point3D(halfWidth, -halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(Point3D(halfWidth, halfLength, 0.0f), Vector3D(0.0f, 0.0f, 1.0f));

		Plane defaultPlane{ Point3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f) };
		auto planeTransform = plane.getTransformToSelf(defaultPlane);

		return generateRenderData(vertices, planeTransform);
	}
}
