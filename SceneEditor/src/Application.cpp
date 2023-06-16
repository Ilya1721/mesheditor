module Application;

import std;

import Window;

namespace MeshEditor
{
	Application::Application(RenderSystem::Window* window) noexcept	:
		mWindow(window)
	{}

	void Application::start() noexcept
	{
		mWindow->start();
	}
}
