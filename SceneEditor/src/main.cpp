#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
	auto mainWindow = Window::createInstance(1200, 628, R"(D:\3DModels\Meshes\Cube\Cube.stl)");
	mainWindow->render();

	return 0;
}