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
		static Scene& getInstance();

		void init(const std::string& meshFilePath);

		const MeshCore::Object3D& getRootObject() const;

	private:
		~Scene() = default;

		void adjustLightPos();

	private:
		MeshCore::Object3D sRootObject;
		static Scene sInstance;
	};
}