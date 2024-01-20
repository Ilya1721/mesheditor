#include "Object3D.h"

#include "Constants.h"

namespace MeshCore
{
	Object3D::Object3D() :
		mParent(nullptr),
		mTransform(1.0f)
	{}

	Object3D::Object3D(Object3D* parent, Mesh&& mesh) :
		mParent(parent),
		mMesh(std::move(mesh)),
		mTransform(1.0f)
	{
		if (parent)
		{
			parent->appendChild(this);
		}
	}

	void Object3D::setParent(Object3D* parent)
	{
		mParent = parent;
	}

	const Mesh& Object3D::getMesh() const
	{
		return mMesh;
	}

	Object3D* Object3D::getParent() const 
	{
		return mParent;
	}

	const std::unordered_set<Object3D*>& Object3D::getChildren() const
	{
		return mChildren;
	}

	RenderData Object3D::getRenderData() const
	{
		return getRenderData(this);
	}

	RenderData Object3D::getRenderData(const Object3D* object) const
	{
		RenderData renderData;
		renderData.append(object->getMesh().getRenderData());
		for (const auto& child : object->getChildren())
		{
			renderData.append(getRenderData(child));
		}

		return renderData;
	}

	const glm::mat4& Object3D::getTransform() const
	{
		return mTransform;
	}

	void Object3D::setTransform(const glm::mat4& transform)
	{
		mTransform = transform;
	}

	void Object3D::appendChild(Object3D* object)
	{
		if (object)
		{
			mChildren.insert(object);
			object->setParent(this);
		}
	}

	void Object3D::removeChild(Object3D* object)
	{
		auto childIt = mChildren.find(object);
		if (childIt != mChildren.end())
		{
			mChildren.erase(childIt);
			object->setParent(nullptr);
		}
	}
}
