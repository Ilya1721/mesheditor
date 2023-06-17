module;
#include "GeometryCore/Matrix.h"
export module Object3D;

import <memory>;
import <unordered_set>;

import Mesh;
import RenderData;

export namespace MeshCore 
{
	class Object3D final
	{
	public:
		Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh = nullptr) noexcept;

		void updateLocalTransform(const Geometry::Matrix4D& transform) noexcept;
		void updateChildrenTransforms() noexcept;
		void updateTransformsGlobally() noexcept;
		const Geometry::Matrix4D& getLocalTransform() const noexcept;

		void setParent(Object3D* parent) noexcept;
		Object3D* getParent() const noexcept;
		const std::unordered_set<Object3D*>& getChildren() const noexcept;
		void appendChild(Object3D* object) noexcept;
		void removeChild(Object3D* object) noexcept;

		void setMesh(std::unique_ptr<Mesh> mesh) noexcept;
		void setLocalTransform(const Geometry::Matrix4D& transform) noexcept;
		const std::unique_ptr<Mesh>& getMesh() const noexcept;

		const RenderData& getRenderData() const noexcept;
		void prepareRenderData() noexcept;

	private:
		Geometry::Matrix4D mLocalTransform;

		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;

		std::unique_ptr<Mesh> mMesh;
		RenderData mRenderData;
		bool mNeedUpdateRenderData;

		int mVertexShader;
		int mFragmentShader;
	};
}

