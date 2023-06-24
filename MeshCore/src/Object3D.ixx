module;
#include "GeometryCore/Matrix.h"
export module Object3D;

import <memory>;
import <unordered_set>;

import Mesh;

export namespace MeshCore 
{
	class Object3D final
	{
	public:
		Object3D() = default;
		Object3D(Object3D* parent, Mesh&& mesh);
		Object3D(const Object3D& other) = delete;
		Object3D(Object3D&& other) = default;
		Object3D& operator=(const Object3D& other) = delete;
		Object3D& operator=(Object3D&& other) = default;

		void setParent(Object3D* parent) noexcept;
		Object3D* getParent() const noexcept;
		const std::unordered_set<Object3D*>& getChildren() const noexcept;
		void appendChild(Object3D* object) noexcept;
		void removeChild(Object3D* object) noexcept;

		const Mesh& getMesh() const noexcept;
		const RenderData getRenderData() const noexcept;

		void updateTransform(const Geometry::Matrix4D& transform) noexcept;
		const Geometry::Matrix4D& getTransform() const noexcept;

	private:
		const RenderData getRenderData(const Object3D* object) const noexcept;

	private:
		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;
		Mesh mMesh;
		Geometry::Matrix4D mTransform;
	};
}

