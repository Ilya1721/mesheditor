import Window;

int main()
{
	auto mainWindow = RenderSystem::Window::createInstance("Mesh Editor", 1200, 628, R"(D:\3D Models\Cube\CubeText.stl)");
	mainWindow->render();

	return 0;
}