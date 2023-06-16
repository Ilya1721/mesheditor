module;
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
export module AABBox;

import Mesh;
import Object3D;

export namespace MeshCore
{
	class AABBox final
	{
	public:
		AABBox() noexcept;
		AABBox(const AABBox& box) = default;
		AABBox(AABBox&& box) = default;
		AABBox& operator=(const AABBox& box) = default;
		AABBox& operator=(AABBox&& box) = default;

		void setFromMesh(const Mesh& mesh) noexcept;
		void setFromObject(const Object3D& object) noexcept;

		void applyTransform(const Geometry::Matrix4D& transform) noexcept;

		Geometry::Vector4D getCenter() const noexcept;
		const Geometry::Vector4D& getMin() const noexcept;
		const Geometry::Vector4D& getMax() const noexcept;
		double getHeight() const noexcept;

	private:
		void init() noexcept;

	private:
		Geometry::Vector4D mMin;
		Geometry::Vector4D mMax;
	};
}

