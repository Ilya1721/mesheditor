module;
#include "GeometryCore/Vector.h"
export module Scene;

import <string>;
import <memory>;

import Mesh;
import Object3D;
import BasicRenderer;
import Viewport;
import PanHandler;
import IEventHandler;
import RenderData;
import RenderSystemConsts;

export namespace RenderSystem
{
	class Scene final : public IEventHandler
	{
	public:
		Scene(const std::string& meshFilePath, std::unique_ptr<BasicRenderer> renderer) noexcept;
		~Scene() noexcept;
		Scene(const Scene& scene) noexcept = delete;
		Scene(Scene&& scene) noexcept = delete;
		Scene& operator=(const Scene& scene) noexcept = delete;
		Scene& operator=(Scene&& scene) noexcept = delete;

		void init() noexcept;
		void setViewport(int x, int y, int width, int height) noexcept;
		void setRenderer(std::unique_ptr<BasicRenderer> renderer) noexcept;
		void setBackgroundColor(const Geometry::Vector4D& backgroundColor = BACKGROUND_COLOR) noexcept;
		void render() noexcept;

		void onMouseMove(const Geometry::Vector2D& mousePos, const MouseButtonsState& mouseButtonsState) noexcept override;
		void onMouseDown(int button, const Geometry::Vector2D& mousePos) noexcept override;
		void onMouseUp(int button) noexcept override;
		void onMouseScroll(double yOffset) noexcept override;
		void onKeyDown(int keyCode) noexcept override;
		void onKeyUp(int keyCode) noexcept override;

	private:
		void initRenderer() noexcept;
		void initViewport() noexcept;

	protected:
		void setRenderData(const MeshCore::RenderData& renderData) noexcept;
		void render(const MeshCore::Object3D& object) noexcept;

	protected:
		unsigned int mVBO;
		unsigned int mVAO;

		std::unique_ptr<BasicRenderer> mRenderer;
		Viewport mViewport;
		PanHandler mPanHandler;
		Geometry::Vector2D mSavedMousePos;
		std::unique_ptr<MeshCore::Object3D> mRootObject;
	};
}