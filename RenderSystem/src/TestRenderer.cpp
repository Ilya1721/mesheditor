module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include "GeometryCore/Vector.h"
module TestRenderer;

namespace RenderSystem
{
	void drawLine(const Geometry::Vector3D& startPoint, const Geometry::Vector3D& endPoint)
	{
		glBegin(GL_LINES);
			glVertex3d(startPoint.x(), startPoint.y(), startPoint.z());
			glVertex3d(endPoint.x(), endPoint.y(), endPoint.z());
		glEnd();
	}
}
