#include "Object3D.h"

#include "GeometryCore/Numeric.h"

#include "Constants.h"

using namespace GeometryCore;

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
		init();
	}

	void Object3D::init()
	{
		calculateBBox(this);

		if (mParent)
		{
			mParent->appendChild(this);
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

	const AABBox& Object3D::getBBox() const
	{
		return mBBox;
	}

	int Object3D::getClosestToCameraIntersectedFaceIndex(const GeometryCore::Ray& ray, const glm::vec3& cameraPos) const
	{
		auto currentClosestIntersection = mMesh.getClosestToCameraIntersectedFace(ray, cameraPos);
		for (const auto& child : mChildren)
		{
			auto childClosestIntersection = child->mMesh.getClosestToCameraIntersectedFace(ray, cameraPos);
			if (currentClosestIntersection.faceIdx == -1 || isCloser(childClosestIntersection.point, currentClosestIntersection.point, cameraPos))
			{
				currentClosestIntersection = childClosestIntersection;
			}
		}

		return currentClosestIntersection.faceIdx;
	}

	void Object3D::calculateBBox(const Object3D* object)
	{
		mBBox.applyMesh(object->getMesh(), object->getTransform());
		for (const auto& child : object->getChildren())
		{
			calculateBBox(child);
		}
	}

	void Object3D::appendChild(Object3D* child)
	{
		if (!child)
		{
			return;
		}

		mChildren.insert(child);
		child->setParent(this);
		child->updateParentBBox(child->mParent);
	}

	void Object3D::removeChild(Object3D* child)
	{
		auto childIt = mChildren.find(child);
		if (childIt != mChildren.end())
		{
			mChildren.erase(childIt);
			child->setParent(nullptr);
			child->recalcParentBBox(child->mParent);
		}
	}

	void Object3D::updateParentBBox(Object3D* parent) const
	{
		if (!parent)
		{
			return;
		}

		parent->mBBox.applyOtherBBox(this->mBBox);
		parent->updateParentBBox(parent->mParent);
	}

	void Object3D::recalcParentBBox(Object3D* parent) const
	{
		if (!parent)
		{
			return;
		}

		parent->calculateBBox(parent);
		recalcParentBBox(parent->mParent);
	}
}
