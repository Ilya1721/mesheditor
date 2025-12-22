#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
  Window mainWindow(
    1200, 628,
    R"(C:\MyPersonalSpace\Dev\Projects\MeshEditor\Misc\TestFiles\Moon\Moon 2K.obj)"
  );
  mainWindow.render();

  return 0;
}