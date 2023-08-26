#include "AABBox.h"

#include <numeric>

namespace MeshCore
{
	AABBox::AABBox()
	{
		init();
	}

	void AABBox::setFromMesh(const Mesh& mesh, const glm::mat4& objectTransform)
	{
		for (const auto& vertex : mesh.getVertices())
		{
			auto transformedVertex = objectTransform * vertex;

			mMin[0] = std::min(mMin.x, transformedVertex.pos.x);
			mMax[0] = std::max(mMax.x, transformedVertex.pos.x);

			mMin[1] = std::min(mMin.y, transformedVertex.pos.y);
			mMax[1] = std::max(mMax.y, transformedVertex.pos.y);

			mMin[2] = std::min(mMin.z, transformedVertex.pos.z);
			mMax[2] = std::max(mMax.z, transformedVertex.pos.z);
		}
	}

	void AABBox::setFromObject(const Object3D& object)
	{
		setFromMesh(object.getMesh(), object.getTransform());
		for (const auto& child : object.getChildren())
		{
			setFromObject(*child);
		}
	}

	void AABBox::init()
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
