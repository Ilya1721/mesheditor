#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
	auto mainWindow = Window::createInstance("Mesh Editor", 1200, 628, R"(D:\3D Models\Cube\CubeText.stl)");
	mainWindow->render();

	return 0;
}