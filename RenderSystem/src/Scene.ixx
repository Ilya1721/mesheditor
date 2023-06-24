module;
#include "GeometryCore/Vector.h"
export module Scene;

import <string>;
import <memory>;

import Object3D;
import Renderer;
import Camera;

export namespace RenderSystem
{
	class Window;

	class Scene final
	{
	public:
		Scene(const std::string& meshFilePath, Window* parentWindow);
		Scene(const Scene& scene) = delete;
		Scene(Scene&& scene) = delete;
		Scene& operator=(const Scene& scene) = delete;
		Scene& operator=(Scene&& scene) = delete;

		void render() noexcept;
		void adjustCamera(float fov);

	private:
		void init();

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}