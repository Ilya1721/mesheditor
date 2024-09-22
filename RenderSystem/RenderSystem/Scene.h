#pragma once

#include <memory>
#include <string>
#include <functional>

#include "MeshCore/Object3D.h"
#include "GeometryCore/Typedefs.h"

namespace RenderSystem
{
	using namespace GeometryCore;

	class Scene
	{
	public:
		Scene(const std::string& meshFilePath);
		Scene(Scene&& scene) = delete;

		MeshCore::RaySurfaceIntersection getClosestIntersection(const Ray& cursorRay, bool intersectSurface);
		MeshCore::Object3D* getPickedObject() const;

		void setPickedObject(MeshCore::Object3D* pickedObject);

		static MeshCore::Object3D& getRootObject();

	private:
		void init(const std::string& meshFilePath);

	private:
		MeshCore::Object3D* mPickedObject;
		static MeshCore::Object3D sRootObject;
	};
}