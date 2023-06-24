module;
#include "GeometryCore/Matrix.h"
module Object3D;

import MeshCoreConsts;
import RenderData;

using namespace Geometry;

namespace MeshCore
{
	Object3D::Object3D(Object3D* parent, Mesh&& mesh) :
		mParent(parent),
		mMesh(std::move(mesh)),
		mTransform(1.0f)
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

	const Mesh& Object3D::getMesh() const noexcept
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

	const RenderData Object3D::getRenderData() const noexcept
	{
		return getRenderData(this);
	}

	const RenderData Object3D::getRenderData(const Object3D* object) const noexcept
	{
		RenderData renderData;
		renderData.append(object->getMesh().getRenderData());
		for (const auto& child : object->getChildren())
		{
			renderData.append(getRenderData(child));
		}

		return renderData;
	}

	void Object3D::updateTransform(const Geometry::Matrix4D& transform) noexcept
	{
		mTransform = mTransform * transform;
	}

	const Geometry::Matrix4D& Object3D::getTransform() const noexcept
	{
		return mTransform;
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
}
