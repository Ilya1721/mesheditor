#pragma once

#include <memory>
#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"

#include "Object3D.h"
#include "Object3DIntersectionData.h"

namespace RenderSystem
{
	using namespace GeometryCore;

	class Scene
	{
	public:
		Scene(const std::string& meshFilePath);
		Scene(Scene&& scene) = delete;

		Object3DIntersectionData getClosestIntersection(const Ray& cursorRay, bool intersectSurface);
		Object3D* getPickedObject() const;

		void setPickedObject(Object3D* pickedObject);

		static Object3D& getRootObject();

	private:
		void init(const std::string& meshFilePath);

	private:
		Object3D* mPickedObject;
		static Object3D sRootObject;
	};
}