#pragma once

#include <vector>
#include <memory>

#include "MeshCore/Mesh.h"
#include "MeshCore/Object3D.h"
#include "BasicRenderer.h"
#include "Viewport.h"

namespace RenderSystem
{
	class Scene
	{
	public:
		__declspec(dllexport) Scene(const std::string& meshFilePath, std::unique_ptr<BasicRenderer> renderer = nullptr) noexcept;

		__declspec(dllexport) virtual ~Scene() noexcept;

		__declspec(dllexport) virtual void init() noexcept;

		__declspec(dllexport) virtual void setViewport(int x, int y, int width, int height) noexcept;

		__declspec(dllexport) virtual void setRenderer(std::unique_ptr<BasicRenderer> renderer) noexcept;

		// __declspec(dllexport) virtual void setMesh(const MeshCore::Mesh& mesh) noexcept;

		__declspec(dllexport) virtual void render() noexcept;

	protected:
		void loadData(const std::vector<MeshCore::Vertex>& vertices) noexcept;

		/*struct MeshData
		{
			MeshCore::Mesh mesh;
			bool needUpdate = true;
		};*/

		unsigned int mVBO;
		unsigned int mVAO;

		/*std::vector<float> mPositions;
		std::vector<float> mNormals;
		std::vector<float> mColors;*/

		std::unique_ptr<BasicRenderer> mRenderer;

		Viewport mViewport;

		std::vector<std::unique_ptr<MeshCore::Object3D>> mObjects;

		// MeshData mMeshData;
	};
}