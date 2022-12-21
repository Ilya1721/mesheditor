#include "pch.h"

#include "Application.h"
#include "RenderSystem/MainWindow.h"
#include "RenderSystem/MainScene.h"

int main()
{
	auto mainScene = std::make_unique<RenderSystem::MainScene>(R"(D:\3D Models\triangle\triangle-text.stl)", std::make_unique<RenderSystem::MainSceneRenderer>());

	auto mainWindow = std::make_unique<RenderSystem::MainWindow>("Mesh Editor", 800, 600);

	mainWindow->setScene(std::move(mainScene));

	MeshEditor::Application app(std::move(mainWindow));

	app.start();

	return 0;
}