#pragma once

#include <vector>
#include <memory>

#include "MeshCore/Mesh.h"
#include "MeshCore/Object3D.h"

#include "BasicRenderer.h"
#include "Constants.h"
#include "Viewport.h"
#include "PanHandler.h"
#include "IEventHandler.h"

namespace RenderSystem
{
	class Scene : public IEventHandler
	{
	public:
		API Scene(const std::string& meshFilePath, std::unique_ptr<BasicRenderer> renderer = nullptr) noexcept;
		API virtual ~Scene() noexcept;
		API Scene(const Scene& scene) noexcept = delete;
		API Scene(Scene&& scene) noexcept = delete;
		API Scene& operator=(const Scene& scene) noexcept = delete;
		API Scene& operator=(Scene&& scene) noexcept = delete;

		API virtual void init() noexcept;

		API virtual void setViewport(int x, int y, int width, int height) noexcept;

		API virtual void setRenderer(std::unique_ptr<BasicRenderer> renderer) noexcept;

		API virtual void setBackgroundColor(const glm::vec4& backgroundColor = Constants::BACKGROUND_COLOR) noexcept;

		API virtual void render() noexcept;

		API virtual void onMouseMove(const glm::vec2& mousePos, const MouseButtonsState& mouseButtonsState) noexcept override;
		API virtual void onMouseDown(int button, const glm::vec2& mousePos) noexcept override;
		API virtual void onMouseUp(int button) noexcept override;
		API virtual void onMouseScroll(double yOffset) noexcept override;

		API virtual void onKeyDown(int keyCode) noexcept override;
		API virtual void onKeyUp(int keyCode) noexcept override;

	private:
		void initRenderer() noexcept;
		void initViewport() noexcept;

	protected:
		void setRenderData(const MeshCore::RenderData& renderData) noexcept;
		void render(const MeshCore::Object3D& object) noexcept;

		unsigned int mVBO;
		unsigned int mVAO;

		std::unique_ptr<BasicRenderer> mRenderer;

		Viewport mViewport;

		PanHandler mPanHandler;

		glm::vec2 mSavedMousePos;

		std::unique_ptr<MeshCore::Object3D> mRootObject;
	};
}