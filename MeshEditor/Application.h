#pragma once

#include <memory>

#include "RenderSystem/Window.h"

namespace MeshEditor
{
	class Application
	{
	public:
		Application(std::unique_ptr<RenderSystem::Window> window) noexcept;
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) = delete;

		void start() noexcept;
		
	private:
		std::unique_ptr<RenderSystem::Window> mWindow;
	};
}
