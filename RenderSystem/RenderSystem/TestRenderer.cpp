#include "TestRenderer.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
	void drawLine(const glm::vec3& startPoint, const glm::vec3& endPoint)
	{
		glBegin(GL_LINES);
			glVertex3d(startPoint.x, startPoint.y, startPoint.z);
			glVertex3d(endPoint.x, endPoint.y, endPoint.z);
		glEnd();
	}
}
