#include "Object3D.h"

#include <glm/gtx/transform.hpp>

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"

#include "Mesh.h"
#include "Constants.h"
#include "TreeWalker.h"
#include "RootRenderDataStorage.h"

using namespace GeometryCore;

namespace MeshCore
{
	std::unordered_map<Object3D*, int> Object3D::sObjectVertexCountMap;

	bool registerNewIntersection(
		const RaySurfaceIntersection& oldIntersection, const RaySurfaceIntersection& newIntersection, const Point3D& rayOrigin
	)
	{
		if (newIntersection.surfaceIndices.empty())
		{
			return false;
		}

		return oldIntersection.surfaceIndices.empty() || isCloser(newIntersection.point, oldIntersection.point, rayOrigin);
	}
}

namespace MeshCore
{
	Object3D::Object3D(std::unique_ptr<Mesh> mesh) :
		mParent(nullptr),
		mMesh(std::move(mesh)),
		mTransform(1.0f)
	{
		init();
	}

	Object3D::~Object3D() = default;

	void Object3D::init()
	{
		mMesh->setParentObject(this);
		mBBox.applyMesh(*mMesh);
	}

	Mesh& Object3D::getMesh()
	{
		return *mMesh;
	}

	const Mesh& Object3D::getMesh() const
	{
		return *mMesh;
	}

	Object3D* Object3D::getParent() const
	{
		return mParent;
	}

	const std::vector<std::unique_ptr<Object3D>>& Object3D::getChildren() const
	{
		return mChildren;
	}

	const glm::mat4& Object3D::getTransform() const
	{
		return mTransform;
	}

	void Object3D::updateTransform(const glm::mat4& transform)
	{
		invokeTransformAction([this, &transform]() {
			mTransform = transform * mTransform;
		}, transform);
	}

	const std::unordered_map<Object3D*, int>& Object3D::getObjectVertexCountMap()
	{
		return sObjectVertexCountMap;
	}

	const AABBox& Object3D::getBBox() const
	{
		return mBBox;
	}

	RaySurfaceIntersection Object3D::findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount) const
	{
		RaySurfaceIntersection finalIntersection;
		TreeWalker walker(this);
		walker.forEach([&ray, &finalIntersection, &intersectSurface, &passedFacesCount, this](const Object3D* object) {
			auto intersection = object->getMesh().findIntersection(ray, intersectSurface, passedFacesCount + object->getMesh().getNumberOfFaces());
			if (registerNewIntersection(finalIntersection, intersection, ray.origin))
			{
				finalIntersection = intersection;
			}
		});

		return finalIntersection;
	}

	std::unique_ptr<Object3D> Object3D::clone()
	{
		auto newObject = std::make_unique<Object3D>();
		newObject->mMesh = mMesh->clone();
		newObject->mBBox = mBBox;
		newObject->mTransform = mTransform;
		newObject->mMesh->setParentObject(newObject.get());

		for (const auto& child : mChildren)
		{
			newObject->addChild(child->clone());
		}

		return newObject;
	}

	void Object3D::addChild(std::unique_ptr<Object3D>&& child)
	{
		child->mParent = this;
		child->propagateRenderDataToRoot();
		child->propagateBBoxToRoot();
		mChildren.emplace_back(std::move(child));
	}

	void Object3D::onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices)
	{
		mBBox.applyMesh(*mMesh);
		propagateBBoxToRoot();
		RootRenderDataStorage::updateRenderData(vertices, sObjectVertexCountMap.at(this));
	}

	void Object3D::moveToOrigin()
	{
		updateTransform(glm::translate(-mBBox.getCenter()));
	}

	void Object3D::invokeTransformAction(const std::function<void()>& action, const glm::mat4& transform)
	{
		action();
		mBBox.applyTransform(transform);
		propagateBBoxToRoot();
	}

	void Object3D::propagateRenderDataToRoot()
	{
		sObjectVertexCountMap.insert({ this, RootRenderDataStorage::getRenderData().getVertexCount()});
		RootRenderDataStorage::appendRenderData(mMesh->getRenderData());
	}

	void Object3D::propagateBBoxToRoot() const
	{
		auto currentObject = this;
		while (currentObject->mParent != nullptr)
		{
			currentObject->mParent->mBBox.applyBBox(currentObject->mBBox);
			currentObject = currentObject->mParent;
		}
	}
}
