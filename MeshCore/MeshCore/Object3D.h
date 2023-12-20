#pragma once

#include <memory>
#include <unordered_set>

#include "GeometryCore/Matrix.h"
#include "Mesh.h"

namespace MeshCore 
{
	class Object3D final
	{
	public:
		Object3D();
		Object3D(Object3D* parent, const Mesh& mesh);
		Object3D(Object3D* parent, Mesh&& mesh);
		Object3D(const Object3D& other) = delete;
		Object3D(Object3D&& other) noexcept = default;
		Object3D& operator=(const Object3D& other) = delete;
		Object3D& operator=(Object3D&& other) noexcept = default;

		bool operator==(const Object3D& other) const = default;

		Object3D* getParent() const;
		const std::unordered_set<Object3D*>& getChildren() const;
		void appendChild(Object3D* object);
		void removeChild(Object3D* object);

		const Mesh& getMesh() const;
		RenderData getRenderData() const;
		const glm::mat4& getTransform() const;
		void setTransform(const glm::mat4& transform);

	private:
		RenderData getRenderData(const Object3D* object) const;
		void setParent(Object3D* parent);

	private:
		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;
		Mesh mMesh;
		glm::mat4 mTransform;
	};
}

