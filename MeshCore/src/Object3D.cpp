#include "pch.h"

#include "Object3D.h"

#include "Constants.h"

namespace MeshCore
{
	Object3D::Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh) noexcept :
		mLocalTransform(1.0f),
		mMesh(std::move(mesh)),
		mParent(parent),
		mNeedUpdateRenderData(true),
		mVertexShader(0),
		mFragmentShader(0)
	{}

	void Object3D::setParent(Object3D* parent) noexcept
	{
		if (mParent) 
		{
			mParent->removeChild(this);
		}

		mParent = parent;
		mParent->appendChild(this);
	}

	void Object3D::setMesh(std::unique_ptr<Mesh> mesh) noexcept
	{
		mMesh = std::move(mesh);
	}

	void Object3D::setLocalTransform(const glm::mat4& transform) noexcept 
	{
		mLocalTransform = transform;
	}

	void Object3D::updateLocalTransform(const glm::mat4& transform) noexcept
	{
		mLocalTransform = transform * mLocalTransform;
	}

	void Object3D::updateChildrenTransforms() noexcept
	{
		for (auto& child : mChildren)
		{
			child->updateLocalTransform(mLocalTransform);
			child->updateChildrenTransforms();
		}
	}

	void Object3D::updateTransformsGlobally() noexcept
	{
		auto currentRoot = this;

		while (currentRoot->getParent() != nullptr)
		{
			currentRoot = currentRoot->getParent();
		}

		currentRoot->updateChildrenTransforms();
	}

	const glm::mat4& Object3D::getLocalTransform() const noexcept
	{
		return mLocalTransform;
	}

	const std::unique_ptr<Mesh>& Object3D::getMesh() const noexcept
	{
		return mMesh;
	}

	Object3D* Object3D::getParent() const noexcept 
	{
		return mParent;
	}

	const std::unordered_set<Object3D*>& Object3D::getChildren() const noexcept
	{
		return mChildren;
	}

	const RenderData& Object3D::getRenderData() const noexcept
	{
		return mRenderData;
	}

	void Object3D::appendChild(Object3D* object) noexcept
	{
		object->setParent(this);
	}

	void Object3D::removeChild(Object3D* object) noexcept
	{
		auto childIt = mChildren.find(object);

		if (childIt != mChildren.end())
		{
			mChildren.erase(childIt);
		}
	}

	ShaderType Object3D::getShaderType() const noexcept
	{
		return ShaderType::BASIC_SHADER;
	}

	void Object3D::prepareRenderData() noexcept
	{
		const auto& vertices = mMesh->getVertices();

		mRenderData.positions.reserve(vertices.size() * Constants::COORDINATES_PER_VERTEX);
		mRenderData.normals.reserve(vertices.size() * Constants::COORDINATES_PER_NORMAL);
		mRenderData.colors.reserve(vertices.size() * Constants::COLOR_COMPONENTS_COUNT);

		static_assert(Constants::COORDINATES_PER_VERTEX == Constants::COORDINATES_PER_NORMAL &&
			Constants::COORDINATES_PER_VERTEX == Constants::COLOR_COMPONENTS_COUNT);

		for (const auto& vertex : vertices)
		{
			for (glm::vec3::length_type coordIdx = 0; coordIdx < Constants::COORDINATES_PER_VERTEX; ++coordIdx)
			{
				mRenderData.positions.push_back(vertex.pos[coordIdx]);
				mRenderData.normals.push_back(vertex.normal[coordIdx]);
				mRenderData.colors.push_back(vertex.color[coordIdx]);
			}
		}

		mNeedUpdateRenderData = false;
	}
}
