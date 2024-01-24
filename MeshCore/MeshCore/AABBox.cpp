#include "AABBox.h"

#include <numeric>

namespace
{
	glm::vec3 getMinVector(const glm::vec3& left, const glm::vec3& right)
	{
		glm::vec3 result{};
		result.x = std::min(left.x, right.x);
		result.y = std::min(left.y, right.y);
		result.z = std::min(left.z, right.z);

		return result;
	}

	glm::vec3 getMaxVector(const glm::vec3& left, const glm::vec3& right)
	{
		glm::vec3 result{};
		result.x = std::max(left.x, right.x);
		result.y = std::max(left.y, right.y);
		result.z = std::max(left.z, right.z);

		return result;
	}
}

namespace MeshCore
{
	AABBox::AABBox()
	{
		clear();
	}

	void AABBox::applyMesh(const Mesh& mesh, const glm::mat4& meshTransform)
	{
		for (const auto& vertex : mesh.getVertices())
		{
			auto transformedVertex = meshTransform * vertex;

			mMin.x = std::min(mMin.x, transformedVertex.pos.x);
			mMax.x = std::max(mMax.x, transformedVertex.pos.x);

			mMin.y = std::min(mMin.y, transformedVertex.pos.y);
			mMax.y = std::max(mMax.y, transformedVertex.pos.y);

			mMin.z = std::min(mMin.z, transformedVertex.pos.z);
			mMax.z = std::max(mMax.z, transformedVertex.pos.z);
		}
	}

	void AABBox::clear()
	{
		constexpr auto floatMax = std::numeric_limits<float>::max();
		constexpr auto floatMin = -floatMax;
		mMax = glm::vec3(floatMin, floatMin, floatMin);
		mMin = glm::vec3(floatMax, floatMax, floatMax);
	}

	void AABBox::applyTransform(const glm::mat4& transform)
	{
		mMin = transform * glm::vec4(mMin, 1.0f);
		mMax = transform * glm::vec4(mMax, 1.0f);
	}

	void AABBox::applyOtherBBox(const AABBox& other)
	{
		mMin = getMinVector(mMin, other.mMin);
		mMax = getMaxVector(mMax, other.mMax);
	}

	glm::vec3 AABBox::getCenter() const
	{
		return (mMin + mMax) * 0.5f;
	}

	const glm::vec3& AABBox::getMin() const
	{
		return mMin;
	}

	const glm::vec3& AABBox::getMax() const
	{
		return mMax;
	}

	float AABBox::getHeight() const
	{
		return mMax.y - mMin.y;
	}
}
