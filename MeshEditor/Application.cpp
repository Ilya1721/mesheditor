#include "pch.h"

#include "Application.h"

namespace MeshEditor
{
	Application::Application(std::unique_ptr<RenderSystem::Window> window) noexcept
		:
		mWindow(std::move(window))
	{}

	void Application::start() noexcept
	{
		mWindow->start();
	}
}
