#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
  Window mainWindow(
    1200, 628,
    R"(C:\MyPersonalSpace\Dev\3DModels\Blender\Sphere\Sphere.glb)"
  );
  mainWindow.render();

  return 0;
}