#include "AABBox.h"

#include <numeric>

namespace
{
	enum class MinMaxOption
	{
		MIN,
		MAX
	};

	glm::vec3 getMinMaxVector(const glm::vec3& left, const glm::vec3& right, MinMaxOption minMaxOption)
	{
		glm::vec3 result{};
		result.x = minMaxOption == MinMaxOption::MIN ? std::min(left.x, right.x) : std::max(left.x, right.x);
		result.y = minMaxOption == MinMaxOption::MIN ? std::min(left.y, right.y) : std::max(left.y, right.y);
		result.z = minMaxOption == MinMaxOption::MIN ? std::min(left.z, right.z) : std::max(left.z, right.z);

		return result;
	}
}

namespace MeshCore
{
	using namespace GeometryCore;

	AABBox::AABBox()
	{
		init();
	}

	void AABBox::init()
	{
		clear();
		calcBBoxPlanes();
	}

	void AABBox::setMinMax(const glm::vec3& min, const glm::vec3& max)
	{
		mMin = min;
		mMax = max;
		calcBBoxPlanes();
	}

	void AABBox::applyMesh(const Mesh& mesh, const glm::mat4& meshTransform)
	{
		glm::vec3 min = mMin;
		glm::vec3 max = mMax;

		for (const auto& vertex : mesh.getVertices())
		{
			auto transformedVertex = meshTransform * vertex;

			min.x = std::min(min.x, transformedVertex.pos.x);
			max.x = std::max(max.x, transformedVertex.pos.x);

			min.y = std::min(min.y, transformedVertex.pos.y);
			max.y = std::max(max.y, transformedVertex.pos.y);

			min.z = std::min(min.z, transformedVertex.pos.z);
			max.z = std::max(max.z, transformedVertex.pos.z);
		}

		setMinMax(min, max);
	}

	void AABBox::clear()
	{
		constexpr auto floatMax = std::numeric_limits<float>::max();
		constexpr auto floatMin = -floatMax;
		glm::vec3 min(floatMax, floatMax, floatMax);
		glm::vec3 max(floatMin, floatMin, floatMin);
		setMinMax(min, max);
	}

	void AABBox::applyTransform(const glm::mat4& transform)
	{
		setMinMax(transform * glm::vec4(mMin, 1.0f), transform * glm::vec4(mMax, 1.0f));
	}

	void AABBox::applyOtherBBox(const AABBox& other)
	{
		setMinMax(getMinMaxVector(mMin, other.mMin, MinMaxOption::MIN), getMinMaxVector(mMax, other.mMax, MinMaxOption::MAX));
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

	bool AABBox::checkIntersectionWithRay(const GeometryCore::Ray& ray) const
	{
		for (const auto& bboxPlane : mBBoxPlanes)
		{
			auto intersectionPoint = ray.findIntersection(bboxPlane);
			if (intersectionPoint.has_value() && isPointInsideBBox(intersectionPoint.value()))
			{
				return true;
			}
		}

		return false;
	}

	bool AABBox::isPointInsideBBox(const glm::vec3& point) const
	{
		for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
		{
			if (point[coordIdx] < mMin[coordIdx] || point[coordIdx] > mMax[coordIdx])
			{
				return false;
			}
		}

		return true;
	}

	void AABBox::calcBBoxPlanes()
	{
		mBBoxPlanes.clear();
		mBBoxPlanes.push_back({ mMin, {-1.0f, 0.0f, 0.0f} });
		mBBoxPlanes.push_back({ mMin, {0.0f, -1.0f, 0.0f} });
		mBBoxPlanes.push_back({ mMin, {0.0f, 0.0f, -1.0f} });
		mBBoxPlanes.push_back({ mMax, {1.0f, 0.0f, 0.0f} });
		mBBoxPlanes.push_back({ mMax, {0.0f, 1.0f, 0.0f} });
		mBBoxPlanes.push_back({ mMax, {0.0f, 0.0f, 1.0f} });
	}
}
