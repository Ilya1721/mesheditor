module;
#include "GeometryCore/Vector.h"
export module Vertex;

export namespace MeshCore
{
	using namespace Geometry;

	struct Vertex
	{
		Vector3D pos;
		Vector3D normal;
		Vector3D color;
	};
}
