module AABBox;

import <numeric>;

using namespace Geometry;

namespace MeshCore
{
	AABBox::AABBox()
	{
		init();
	}

	void AABBox::setFromMesh(const Mesh& mesh)
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

	void AABBox::setFromObject(const Object3D& object)
	{
		setFromMesh(object.getMesh());
		for (const auto& child : object.getChildren())
		{
			setFromObject(*child);
		}
	}

	void AABBox::init()
	{
		constexpr auto floatMax = std::numeric_limits<float>::max();
		constexpr auto floatMin = -floatMax;
		mMax = Vector3D(floatMin, floatMin, floatMin);
		mMin = Vector3D(floatMax, floatMax, floatMax);
	}

	void AABBox::applyTransform(const Matrix4D& transform)
	{
		mMin = transform * Geometry::Vector4D(mMin, 1.0f);
		mMax = transform * Geometry::Vector4D(mMax, 1.0f);
	}

	Vector3D AABBox::getCenter() const
	{
		return (mMin + mMax) / 2.0f;
	}

	const Vector3D& AABBox::getMin() const
	{
		return mMin;
	}

	const Vector3D& AABBox::getMax() const
	{
		return mMax;
	}

	float AABBox::getHeight() const
	{
		return mMax.y() - mMin.y();
	}
}
