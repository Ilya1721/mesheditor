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
		AABBox();
		AABBox(const AABBox& box) = default;
		AABBox(AABBox&& box) noexcept = default;
		AABBox& operator=(const AABBox& box) = default;
		AABBox& operator=(AABBox&& box) noexcept = default;

		void setFromMesh(const Mesh& mesh);
		void setFromObject(const Object3D& object);

		void applyTransform(const Geometry::Matrix4D& transform);

		Geometry::Vector3D getCenter() const;
		const Geometry::Vector3D& getMin() const;
		const Geometry::Vector3D& getMax() const;
		float getHeight() const;

	private:
		void init();

	private:
		Geometry::Vector3D mMin;
		Geometry::Vector3D mMax;
	};
}

