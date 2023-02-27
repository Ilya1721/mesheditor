#include "pch.h"

#include "Application.h"

int main()
{
	auto mainScene = std::make_unique<RenderSystem::Scene>(R"(D:\3D Models\Cube\CubeText.stl)", std::make_unique<RenderSystem::BasicRenderer>());
	auto mainWindow = RenderSystem::Window::createInstance("Mesh Editor", 1280, 960);
	mainWindow->setScene(std::move(mainScene));

	MeshEditor::Application app(mainWindow);
	app.start();

	return 0;
}