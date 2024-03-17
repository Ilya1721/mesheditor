#pragma once

#include <unordered_set>

#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

namespace MeshCore
{
	struct HalfEdge;

	struct Vertex
	{
		Vertex() = default;
		Vertex(const glm::vec3& pos, const glm::vec3& normal);

		HalfEdge* halfEdge = nullptr;

		void setPos(const glm::vec3& pos);
		void setNormal(const glm::vec3& normal);

		const glm::vec3& pos() const;
		const glm::vec3& normal() const;

		bool operator==(const Vertex& other) const;
		friend Vertex operator*(const glm::mat4& transform, const Vertex& vertex);

	protected:
		glm::vec3 mPos{};
		glm::vec3 mNormal{};
	};

	struct OriginalVertexData
	{
		Vertex* vertex;
		int index;
	};

	struct UniqueVertex : Vertex
	{
		UniqueVertex(Vertex& originalVertex, int originalVertexIndex);

		void setPos(const glm::vec3& pos);
		void setNormal(const glm::vec3& normal);

		std::vector<OriginalVertexData> originalVertices;
		std::unordered_set<glm::vec3> adjacentFacesNormals;

	private:
		void updateOriginalVertices();
	};

	using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}
