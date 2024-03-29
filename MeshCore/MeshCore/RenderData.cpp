#include "RenderData.h"

#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"

#include "Vertex.h"
#include "Constants.h"

namespace
{
	using namespace MeshCore;

	RenderData getRenderData(std::vector<Vertex>& vertices, const glm::mat4& transform)
	{
		RenderData renderData;
		for (auto& vertex : vertices)
		{
			vertex = transform * vertex;
			renderData.append(vertex);
		}

		return renderData;
	}
}

namespace MeshCore
{
	void RenderData::append(const RenderData& other)
	{
		positions.insert(std::end(positions), std::cbegin(other.positions), std::cend(other.positions));
		normals.insert(std::end(normals), std::cbegin(other.normals), std::cend(other.normals));
		prepareCompactData();
	}

	void RenderData::append(const Vertex& vertex)
	{
		for (int coordIdx = 0; coordIdx < COORDINATES_PER_VERTEX; ++coordIdx)
		{
			positions.emplace_back(vertex.pos()[coordIdx]);
			normals.emplace_back(vertex.normal()[coordIdx]);
		}
		prepareCompactData();
	}

	void RenderData::updateVertex(const OriginalVertexData& vertexData)
	{
		for (int compactDataIdx = vertexData.index * 3, coordIdx = 0; coordIdx < 3; ++compactDataIdx, ++coordIdx)
		{
			mCompactData[compactDataIdx] = vertexData.vertex->pos()[coordIdx];
			mCompactData[compactDataIdx + positions.size()] = vertexData.vertex->normal()[coordIdx];
		}
	}

	void RenderData::reserveMemory(size_t numberOfElements)
	{
		positions.reserve(numberOfElements);
		normals.reserve(numberOfElements);
	}

	void RenderData::prepareCompactData()
	{
		std::vector<float> data (std::cbegin(positions), std::cend(positions));
		data.insert(std::end(data), std::cbegin(normals), std::cend(normals));
		mCompactData = data;
	}

	const std::vector<float>& RenderData::getCompactData() const
	{
		return mCompactData;
	}

	RenderData RenderData::createRenderData(const GeometryCore::Ray& ray, float length)
	{
		RenderData renderData;
		renderData.append({ ray.origin, ray.direction });
		renderData.append({ ray.origin + ray.direction * length, ray.direction });
		
		return renderData;
	}

	RenderData RenderData::createRenderData(const GeometryCore::Line& line, bool withArrowHead)
	{
		std::vector<Vertex> vertices;
		auto halfLength = glm::length(line.end - line.start) * 0.5f;
		GeometryCore::Line defaultLine{ glm::vec3(0.0f, -halfLength, 0.0f), glm::vec3(0.0f, halfLength, 0.0f) };
		vertices.emplace_back(defaultLine.start, glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));

		if (withArrowHead) 
		{
			auto tangent = glm::tan(glm::radians(ARROW_HEAD_ANGLE));
			auto x = tangent * halfLength * ARROW_HEAD_LENGTH_KOEF;
			auto y = defaultLine.end.y - (x / tangent);
			vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(glm::vec3(-x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));
			vertices.emplace_back(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		return getRenderData(vertices, line.getLineToLineTransform(defaultLine));
	}

	RenderData RenderData::createRenderData(const GeometryCore::Plane& plane, float width, float length)
	{
		std::vector<Vertex> vertices;
		auto halfWidth = width * 0.5f;
		auto halfLength = length * 0.5f;
		vertices.emplace_back(glm::vec3(halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(glm::vec3(-halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(glm::vec3(-halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(glm::vec3(-halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(glm::vec3(halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		vertices.emplace_back(glm::vec3(halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		GeometryCore::Plane defaultPlane{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
		auto planeTransform = plane.getPlaneToPlaneTransform(defaultPlane);

		return getRenderData(vertices, planeTransform);
	}
}
