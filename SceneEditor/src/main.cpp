#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
	Window mainWindow(1200, 628, R"(C:\MyPersonalSpace\Dev\3DModels\STL\Cube\Cube.stl)");
	mainWindow.render();

	return 0;
}