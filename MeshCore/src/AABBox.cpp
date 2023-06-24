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
			mMin.setX(std::min(mMin.x(), vertex.pos.x()));
			mMax.setX(std::max(mMax.x(), vertex.pos.x()));

			mMin.setY(std::min(mMin.y(), vertex.pos.y()));
			mMax.setY(std::max(mMax.y(), vertex.pos.y()));

			mMin.setZ(std::min(mMin.z(), vertex.pos.z()));
			mMax.setZ(std::max(mMax.z(), vertex.pos.z()));
		}
	}

	void AABBox::setFromObject(const Object3D& object) noexcept
	{
		setFromMesh(object.getMesh());
		for (const auto& child : object.getChildren())
		{
			setFromObject(*child);
		}
	}

	void AABBox::init() noexcept
	{
		constexpr auto floatMin = std::numeric_limits<float>::min();
		constexpr auto floatMax = std::numeric_limits<float>::max();
		mMax = Vector4D(floatMin, floatMin, floatMin, 1.0f);
		mMin = Vector4D(floatMax, floatMax, floatMax, 1.0f);
	}

	void AABBox::applyTransform(const Matrix4D& transform) noexcept
	{
		mMin = mMin * transform;
		mMax = mMax * transform;
	}

	Vector4D AABBox::getCenter() const noexcept
	{
		return (mMin + mMax) / 2.0f;
	}

	const Vector4D& AABBox::getMin() const noexcept
	{
		return mMin;
	}

	const Vector4D& AABBox::getMax() const noexcept
	{
		return mMax;
	}

	float AABBox::getHeight() const noexcept
	{
		return mMax.y() - mMin.y();
	}
}
