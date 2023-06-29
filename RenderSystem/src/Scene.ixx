module;
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
export module Scene;

import <string>;
import <memory>;

import Object3D;
import Renderer;
import AABBox;
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

		void setProjectionMatrix(const Geometry::Matrix4D& projectionMatrix);

		const Geometry::Matrix4D& getModelMatrix() const;
		const Geometry::Matrix4D& getViewMatrix() const;

		void render();
		void adjust(float fov);
		void pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint);

	private:
		void init();
		void initRenderBuffer();
		void initLighting();
		void adjustCamera(const MeshCore::AABBox& bbox, float fov);
		void adjustLightPos(const MeshCore::AABBox& bbox);
		const Geometry::Matrix4D& getProjectionMatrix() const;

	private:
		Window* mParentWindow;
		Renderer mRenderer;
		Camera mCamera;
		MeshCore::Object3D mRootObject;
	};
}