#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
	auto mainWindow = Window::createInstance(1200, 628, R"(D:\3D Models\Sphere\SphereLowQuality.stl)");
	mainWindow->render();

	return 0;
}