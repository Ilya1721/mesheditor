import Window;

int main()
{
	auto mainWindow = RenderSystem::Window::createInstance("Mesh Editor", 1280, 960, R"(D:\3D Models\Triangle\triangle-text.stl)");
	mainWindow->render();

	return 0;
}