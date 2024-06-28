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
	Object3D::Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh) :
		mParent(parent),
		mMesh(std::move(mesh)),
		mTransform(1.0f)
	{
		init();
	}

	Object3D::~Object3D() = default;

	void Object3D::init()
	{
		calculateBBox();

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
		return *mMesh;
	}

	Mesh& Object3D::getMesh()
	{
		return *mMesh;
	}

	Object3D* Object3D::getParent() const 
	{
		return mParent;
	}

	const std::unordered_set<Object3D*>& Object3D::getChildren() const
	{
		return mChildren;
	}

	RenderData Object3D::getRenderData()
	{
		RenderData renderData;
		TreeWalker<Object3D> walker(this);
		walker.forEach([&renderData](Object3D* object) {
			renderData.append(object->getMesh().getRenderData());
		});

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

	std::optional<Point3D> Object3D::findIntersection(const Ray& ray)
	{
		auto startIntersection = std::make_optional<Point3D>();
		TreeWalker walker(this);
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
			auto intersection = object->getMesh().findIntersection(ray, intersectSurface, passedFacesCount + mMesh->getNumberOfFaces());
			if (startIntersection.surfaceIndices.empty() || isCloser(intersection.point, startIntersection.point, ray.origin))
			{
				startIntersection = intersection;
			}
		});

		return startIntersection;
	}

	void Object3D::calculateBBox()
	{
		TreeWalker walker(this);
		walker.forEach([this](Object3D* object) {
			mBBox.applyMesh(object->getMesh(), object->getTransform());
		});
		setTransform(glm::translate(-mBBox.getCenter()));
	}

	void Object3D::appendChild(Object3D* child)
	{
		if (child)
		{
			mChildren.insert(child);
			child->setParent(this);
			child->updateParentBBox(child->mParent);
		}
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
		if (parent)
		{
			parent->mBBox.applyOtherBBox(this->mBBox);
			parent->updateParentBBox(parent->mParent);
		}
	}

	void Object3D::recalcParentBBox(Object3D* parent) const
	{
		if (parent)
		{
			parent->calculateBBox();
			recalcParentBBox(parent->mParent);
		}
	}
}
