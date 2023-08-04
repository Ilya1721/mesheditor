#pragma once

#include "GeometryCore/Matrix.h"

#include <memory>
#include <unordered_set>

#include "Mesh.h"

namespace MeshCore 
{
	class Object3D final
	{
	public:
		Object3D() = default;
		Object3D(Object3D* parent, Mesh&& mesh);
		Object3D(const Object3D& other) = delete;
		Object3D(Object3D&& other) noexcept = default;
		Object3D& operator=(const Object3D& other) = delete;
		Object3D& operator=(Object3D&& other) noexcept = default;

		void setParent(Object3D* parent);
		Object3D* getParent() const;
		const std::unordered_set<Object3D*>& getChildren() const;
		void appendChild(Object3D* object);
		void removeChild(Object3D* object);

		const Mesh& getMesh() const;
		const RenderData getRenderData() const;

		void updateTransform(const glm::mat4& transform);
		const glm::mat4& getTransform() const;

	private:
		const RenderData getRenderData(const Object3D* object) const;

	private:
		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;
		Mesh mMesh;
		glm::mat4 mTransform;
	};
}

