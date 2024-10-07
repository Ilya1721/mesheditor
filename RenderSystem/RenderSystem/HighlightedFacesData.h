#pragma once

#include <vector>

namespace RenderSystem
{
    class Object3D;

    struct HighlightedFacesData
    {
        std::vector<int> facesIndices;
        const Object3D* parentObject = nullptr;
    };
}
