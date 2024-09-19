#include "RenderSystem/Window.h"

using namespace RenderSystem;

namespace
{
	Window* gWindow = &Window::getInstance();
}

int main()
{
	gWindow->init(1200, 628, R"(D:\3DModels\STL\Cube\Cube.stl)");
	gWindow->render();

	return 0;
}