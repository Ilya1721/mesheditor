import <memory>;

import Scene;
import Window;
import Application;
import BasicRenderer;

int main()
{
	auto mainScene = std::make_unique<RenderSystem::Scene>(R"(D:\3D Models\Triangle\triangle-text.stl)", std::make_unique<RenderSystem::BasicRenderer>());
	auto mainWindow = RenderSystem::Window::createInstance("Mesh Editor", 1280, 960);
	mainWindow->setScene(std::move(mainScene));

	MeshEditor::Application app(mainWindow);
	app.start();

	return 0;
}