#pragma once

#include <memory>
#include <unordered_set>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "RenderData.h"
#include "ShaderType.h"

namespace MeshCore 
{
	class Object3D
	{
	public:
		API Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh = nullptr) noexcept;
		API virtual ~Object3D() noexcept = default;
		API Object3D(const Object3D& object) noexcept = default;
		API Object3D(Object3D&& object) noexcept = default;
		API Object3D& operator=(const Object3D& object) noexcept = default;
		API Object3D& operator=(Object3D&& object) noexcept = default;

		API void setParent(Object3D* parent) noexcept;
		API void setMesh(std::unique_ptr<Mesh> mesh) noexcept;
		API void setLocalTransform(const glm::mat4& transform) noexcept;

		API void updateLocalTransform(const glm::mat4& transform) noexcept;
		API void updateChildrenTransforms() noexcept;
		API void updateTransformsGlobally() noexcept;

		API const glm::mat4& getLocalTransform() const noexcept;
		API const std::unique_ptr<Mesh>& getMesh() const noexcept;
		API Object3D* getParent() const noexcept;
		API const std::unordered_set<Object3D*>& getChildren() const noexcept;
		API const RenderData& getRenderData() const noexcept;

		API void appendChild(Object3D* object) noexcept;
		API void removeChild(Object3D* object) noexcept;

		API virtual ShaderType getShaderType() const noexcept;
		API virtual void prepareRenderData() noexcept;

	private:
		glm::mat4 mLocalTransform;

		std::unique_ptr<Mesh> mMesh;

		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;

		RenderData mRenderData;

		bool mNeedUpdateRenderData;

		int mVertexShader;
		int mFragmentShader;
	};
}

