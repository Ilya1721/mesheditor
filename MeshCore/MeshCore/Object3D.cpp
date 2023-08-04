#include "Object3D.h"

#include "RenderData.h"
#include "Constants.h"

namespace MeshCore
{
	Object3D::Object3D(Object3D* parent, Mesh&& mesh) :
		mParent(parent),
		mMesh(std::move(mesh)),
		mTransform(1.0f)
	{}

	void Object3D::setParent(Object3D* parent)
	{
		if (mParent) 
		{
			mParent->removeChild(this);
		}

		mParent = parent;
		mParent->appendChild(this);
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

	const RenderData Object3D::getRenderData() const
	{
		return getRenderData(this);
	}

	const RenderData Object3D::getRenderData(const Object3D* object) const
	{
		RenderData renderData;
		renderData.append(object->getMesh().getRenderData());
		for (const auto& child : object->getChildren())
		{
			renderData.append(getRenderData(child));
		}

		return renderData;
	}

	void Object3D::updateTransform(const glm::mat4& transform)
	{
		mTransform = mTransform * transform;
	}

	const glm::mat4& Object3D::getTransform() const
	{
		return mTransform;
	}

	void Object3D::appendChild(Object3D* object)
	{
		object->setParent(this);
	}

	void Object3D::removeChild(Object3D* object)
	{
		auto childIt = mChildren.find(object);
		if (childIt != mChildren.end())
		{
			mChildren.erase(childIt);
		}
	}
}
