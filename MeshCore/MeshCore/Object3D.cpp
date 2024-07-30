#include "Object3D.h"

#include <glm/gtx/transform.hpp>

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"

#include "Mesh.h"
#include "Constants.h"
#include "Intersection.h"
#include "RenderData.h"
#include "TreeWalker.h"

using namespace GeometryCore;

namespace MeshCore
{
	std::vector<Object3D*> Object3D::sAllObjects;
	std::unordered_map<Object3D*, int> Object3D::sObjectRenderDataOffsetMap;
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
	}

	void Object3D::setParent(Object3D* parent)
	{
		mParent = parent;
	}

	Mesh& Object3D::getMesh()
	{
		return *mMesh;
	}

	Object3D* Object3D::getParent() const
	{
		return mParent;
	}

	const std::unordered_set<std::unique_ptr<Object3D>>& Object3D::getChildren() const
	{
		return mChildren;
	}

	const RenderData& Object3D::getRenderData() const
	{
		return mRenderData;
	}

	const glm::mat4& Object3D::getTransform() const
	{
		return mTransform;
	}

	void Object3D::setTransform(const glm::mat4& transform)
	{
		invokeTransformAction([this, &transform]() {
			mTransform = transform;
			}, transform);
	}

	void Object3D::updateTransform(const glm::mat4& transform)
	{
		invokeTransformAction([this, &transform]() {
			mTransform = transform * mTransform;
			}, transform);
	}

	void Object3D::updateRenderData(const OriginalVertexData& vertexData)
	{
		getRoot()->mRenderData.updateVertex(vertexData, sObjectRenderDataOffsetMap.at(this));
	}

	const std::vector<Object3D*>& Object3D::getAllObjects()
	{
		return sAllObjects;
	}

	const AABBox& Object3D::getBBox() const
	{
		return mBBox;
	}

	std::optional<Point3D> Object3D::findIntersection(const Ray& ray)
	{
		auto startIntersection = std::make_optional<Point3D>();
		TreeWalker<Object3D> walker(this);
		walker.forEach([&ray, &startIntersection](Object3D* object) {
			auto intersection = object->getMesh().findIntersection(ray);
			if (intersection && (!startIntersection.has_value() || isCloser(intersection.value(), startIntersection.value(), ray.origin)))
			{
				startIntersection = intersection;
			}
		});

		return startIntersection;
	}

	RaySurfaceIntersection Object3D::findIntersection(const Ray& ray, bool intersectSurface, int passedFacesCount)
	{
		RaySurfaceIntersection startIntersection;
		TreeWalker walker(this);
		walker.forEach([&ray, &startIntersection, &intersectSurface, &passedFacesCount, this](Object3D* object) {
			auto intersection = object->getMesh().findIntersection(ray, intersectSurface, passedFacesCount + object->getMesh().getNumberOfFaces());
			if (startIntersection.surfaceIndices.empty() || isCloser(intersection.point, startIntersection.point, ray.origin))
			{
				startIntersection = intersection;
			}
		});

		return startIntersection;
	}

	Object3D* Object3D::clone()
	{
		TreeWalker walker(this);
		return nullptr;
	}

	void Object3D::addChild(std::unique_ptr<Object3D>&& child)
	{
		child->setParent(this);
		child->propagateRenderDataToRoot();
		mChildren.insert(std::move(child));
		calculateBBox();
	}

	void Object3D::calculateBBox()
	{
		TreeWalker walker(this);
		walker.forEach([this](Object3D* object) {
			mBBox.applyMesh(object->getMesh());
			object->setTransform(glm::translate(-mBBox.getCenter()));
		});
	}

	void Object3D::invokeTransformAction(const std::function<void()>& action, const glm::mat4& transform)
	{
		action();
		mBBox.applyTransform(transform);
	}

	void Object3D::propagateRenderDataToRoot()
	{
		auto root = getRoot();
		sObjectRenderDataOffsetMap.insert({ this, root->mRenderData.getCompactData().size() });
		root->mRenderData.append(mMesh->getRenderData());
		sAllObjects.push_back(this);
	}

	Object3D* Object3D::getRoot()
	{
		auto currentObject = this;
		while (currentObject->getParent() != nullptr)
		{
			currentObject = currentObject->getParent();
		}

		return currentObject;
	}
}
