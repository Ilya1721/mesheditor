#pragma once

#include <memory>
#include <unordered_set>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "RenderData.h"

namespace MeshCore 
{
	class Object3D
	{
	public:
		Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh = nullptr) noexcept;

		void setParent(Object3D* parent) noexcept;
		void setMesh(std::unique_ptr<Mesh> mesh) noexcept;
		void setLocalTransform(const glm::mat4& transform) noexcept;

		void updateLocalTransform(const glm::mat4& transform) noexcept;
		void updateChildrenTransforms() noexcept;
		void updateTransformsGlobally() noexcept;

		const glm::mat4& getLocalTransform() const noexcept;
		const std::unique_ptr<Mesh>& getMesh() const noexcept;
		Object3D* getParent() const noexcept;
		const std::unordered_set<Object3D*>& getChildren() const noexcept;
		const RenderData& getRenderData() const noexcept;

		void appendChild(Object3D* object) noexcept;
		void removeChild(Object3D* object) noexcept;

		void prepareRenderData() noexcept;

	private:
		glm::mat4 mLocalTransform;

		std::unique_ptr<Mesh> mMesh;

		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;

		RenderData mRenderData;

		bool mNeedUpdateRenderData;
	};
}

