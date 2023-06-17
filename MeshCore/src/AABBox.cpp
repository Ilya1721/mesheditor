module AABBox;

import <numeric>;

using namespace Geometry;

namespace MeshCore
{
	AABBox::AABBox() noexcept
	{
		init();
	}

	void AABBox::setFromMesh(const Mesh& mesh) noexcept
	{
		for (const auto& vertex : mesh.getVertices())
		{
			const auto& pos = vertex.pos;

			mMin.setX(std::min(mMin.x(), pos.x()));
			mMax.setX(std::max(mMax.x(), pos.x()));

			mMin.setY(std::min(mMin.y(), pos.y()));
			mMax.setY(std::max(mMax.y(), pos.y()));

			mMin.setZ(std::min(mMin.z(), pos.z()));
			mMax.setZ(std::max(mMax.z(), pos.z()));
		}
	}

	void AABBox::setFromObject(const Object3D& object) noexcept
	{
		setFromMesh(*object.getMesh());
		applyTransform(object.getLocalTransform());

		for (const auto& child : object.getChildren())
		{
			setFromObject(child);
		}
	}

	void AABBox::init() noexcept
	{
		constexpr auto doubleMin = std::numeric_limits<double>::min();
		constexpr auto doubleMax = std::numeric_limits<double>::max();
		mMax = Vector4D(doubleMin, doubleMin, doubleMin, 1.0);
		mMin = Vector4D(doubleMax, doubleMax, doubleMax, 1.0);
	}

	void AABBox::applyTransform(const Matrix4D& transform) noexcept
	{
		mMin = mMin * transform;
		mMax = mMax * transform;
	}

	Vector4D AABBox::getCenter() const noexcept
	{
		return (mMin + mMax) / 2.0;
	}

	const Vector4D& AABBox::getMin() const noexcept
	{
		return mMin;
	}

	const Vector4D& AABBox::getMax() const noexcept
	{
		return mMax;
	}

	double AABBox::getHeight() const noexcept
	{
		return mMax.y() - mMin.y();
	}
}
