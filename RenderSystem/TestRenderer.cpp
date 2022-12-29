#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif

#include "glad.h"

#include "TestRenderer.h"

namespace RenderSystem
{
	void TestRenderer::drawLine(const glm::vec3& startPoint, const glm::vec3& endPoint) noexcept
	{
		glBegin(GL_LINES);
			glVertex3f(startPoint.x, startPoint.y, startPoint.z);
			glVertex3f(endPoint.x, endPoint.y, endPoint.z);
		glEnd();
	}
}
