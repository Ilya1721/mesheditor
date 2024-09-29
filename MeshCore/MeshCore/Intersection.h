#pragma once

#include <vector>
#include <optional>

#include "Surface.h"

namespace MeshCore
{
	struct IntersectionTempData
	{
		int intersectedFaceIdx;
		int passedFacesCount;
	};

	struct MeshIntersectionData
	{
		std::vector<int> interestedFacesIndices;
		Point3D intersectionPoint;
		Surface intersectedSurface;
	};
}
