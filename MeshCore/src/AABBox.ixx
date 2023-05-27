module;
#include "SolutionMacros.h"
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
export module AABBox;

export namespace MeshCore
{
	using namespace Geometry;

	class API AABBox
	{
	public:
		AABBox() noexcept;
		AABBox(const AABBox& box) = default;
		AABBox(AABBox&& box) = default;
		AABBox& operator=(const AABBox& box) = default;
		AABBox& operator=(AABBox&& box) = default;
		virtual ~AABBox() noexcept = default;

		void setFromMesh(const Mesh& mesh) noexcept;
		void setFromObject(const Object3D& object) noexcept;

		void applyTransform(const Matrix4D& transform) noexcept;

		Vector3D getCenter() const noexcept;
		const Vector3D& getMin() const noexcept;
		const Vector3D& getMax() const noexcept;
		double getHeight() const noexcept;

	private:
		void init() noexcept;

	private:
		Vector4D mMin;
		Vector4D mMax;
	};
}

