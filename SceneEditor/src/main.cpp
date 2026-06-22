#include "RenderSystem/Window.h"

using namespace RenderSystem;

int main()
{
  auto modelsPath = std::filesystem::current_path() / "ThirdParty" / "Models";
  auto modelPath = modelsPath / "Cube" / "Obj" / "CubeWithHole.obj";
  Window mainWindow(1200, 628, modelPath.string());
  mainWindow.render();

  return 0;
}