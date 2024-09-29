#include "Object3D.h"

#include <glm/gtx/transform.hpp>

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"

#include "MeshCore/Mesh.h"
#include "MeshCore/Constants.h"
#include "MeshCore/TreeWalker.h"

#include "RootRenderDataStorage.h"

using namespace GeometryCore;

namespace RenderSystem
{
	std::unordered_map<const Object3D*, int> Object3D::sObjectVertexCountMap;
}

namespace RenderSystem
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
		prepareRenderData();
		mBBox.applyMesh(*mMesh);
	}

	void Object3D::prepareRenderData()
	{
		for (const auto& vertex : mMesh->getVertices())
		{
			mRenderData.append(vertex);
		}
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

	const std::unordered_map<const Object3D*, int>& Object3D::getObjectVertexCountMap()
	{
		return sObjectVertexCountMap;
	}

	const AABBox& Object3D::getBBox() const
	{
		return mBBox;
	}

	const RenderData& Object3D::getRenderData() const
	{
		return mRenderData;
	}

	Object3DIntersectionData Object3D::findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount)
	{
		Object3DIntersectionData intersectionData;
		TreeWalker walker(this);
		walker.forEach([&ray, &intersectionData, &intersectSurface, &passedFacesCount, this](Object3D* object) {
			const auto numberOfFaces = object->getMesh().getFaces().size() + passedFacesCount;
			auto invertedRay = glm::inverse(object->getTransform()) * ray;
			auto meshIntersectionData = object->getMesh().findIntersection(invertedRay, intersectSurface, numberOfFaces);
			intersectionData.intersectedObject = object;
			intersectionData.intersection.setClosest(meshIntersectionData, invertedRay.origin);
		});

		return intersectionData;
	}

	std::unique_ptr<Object3D> Object3D::clone()
	{
		auto newObject = std::make_unique<Object3D>();
		newObject->mMesh = mMesh->clone();
		newObject->mBBox = mBBox;
		newObject->mTransform = mTransform;
		newObject->mParent = mParent;
		newObject->mRenderData = mRenderData;

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
		RootRenderDataStorage::appendRenderData(mRenderData);
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
