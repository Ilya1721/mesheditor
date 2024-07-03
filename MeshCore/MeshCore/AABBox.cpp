#include "AABBox.h"

#include <numeric>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Plane.h"
#include "Mesh.h"
#include "Vertex.h"

using namespace GeometryCore;

namespace
{
	enum class MinMaxOption
	{
		MIN,
		MAX
	};

	Point3D getMinMaxPoint(const Point3D& left, const Point3D& right, MinMaxOption minMaxOption)
	{
		Point3D result{};
		result.x = minMaxOption == MinMaxOption::MIN ? std::min(left.x, right.x) : std::max(left.x, right.x);
		result.y = minMaxOption == MinMaxOption::MIN ? std::min(left.y, right.y) : std::max(left.y, right.y);
		result.z = minMaxOption == MinMaxOption::MIN ? std::min(left.z, right.z) : std::max(left.z, right.z);

		return result;
	}
}

namespace MeshCore
{
	AABBox::AABBox()
	{
		init();
	}

	void AABBox::init()
	{
		clear();
		calcBBoxPlanes();
	}

	void AABBox::setMinMax(const Point3D& min, const Point3D& max)
	{
		mMin = min;
		mMax = max;
		calcBBoxPlanes();
	}

	void AABBox::applyMesh(const Mesh& mesh)
	{
		Point3D min = mMin;
		Point3D max = mMax;

		for (const auto& vertex : mesh.getVertices())
		{
			min.x = std::min(min.x, vertex.pos.x);
			max.x = std::max(max.x, vertex.pos.x);

			min.y = std::min(min.y, vertex.pos.y);
			max.y = std::max(max.y, vertex.pos.y);

			min.z = std::min(min.z, vertex.pos.z);
			max.z = std::max(max.z, vertex.pos.z);
		}

		setMinMax(min, max);
	}

	void AABBox::clear()
	{
		constexpr auto floatMax = std::numeric_limits<float>::max();
		constexpr auto floatMin = -floatMax;
		Point3D min(floatMax, floatMax, floatMax);
		Point3D max(floatMin, floatMin, floatMin);
		setMinMax(min, max);
	}

	void AABBox::applyOtherBBox(const AABBox& other)
	{
		setMinMax(getMinMaxPoint(mMin, other.mMin, MinMaxOption::MIN), getMinMaxPoint(mMax, other.mMax, MinMaxOption::MAX));
	}

	Point3D AABBox::getCenter() const
	{
		return (mMin + mMax) * 0.5f;
	}

	const Point3D& AABBox::getMin() const
	{
		return mMin;
	}

	const Point3D& AABBox::getMax() const
	{
		return mMax;
	}

	float AABBox::getHeight() const
	{
		return mMax.y - mMin.y;
	}

	std::optional<Point3D> AABBox::findIntersection(const Ray& ray) const
	{
		for (auto& bboxPlane : mBBoxPlanes)
		{
			auto intersectionPoint = bboxPlane.findIntersection(ray);
			if (intersectionPoint.has_value() && isPointInsideBBox(intersectionPoint.value()))
			{
				return intersectionPoint;
			}
		}

		return {};
	}

	bool AABBox::isPointInsideBBox(const Point3D& point) const
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
