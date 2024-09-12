#include "RenderLogger.h"

#include <iostream>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
	void printOpenGLErrorMessage()
	{
		GLenum error{};

		while ((error = glGetError()) && error != GL_NO_ERROR)
		{
			switch (error)
			{
				case GL_INVALID_ENUM:
				{
					std::cerr << "An unacceptable value is specified for an enumerated argument\n";
					break;
				}
				case GL_INVALID_VALUE:
				{
					std::cerr << "A numeric argument is out of range\n";
					break;
				}
				case GL_INVALID_OPERATION:
				{
					std::cerr << "The specified operation is not allowed in the current state\n";
					break;
				}
				case GL_INVALID_FRAMEBUFFER_OPERATION:
				{
					std::cerr << "The framebuffer object is not complete\n";
					break;
				}
				case GL_OUT_OF_MEMORY:
				{
					std::cerr << "There is not enough memory left to execute the command\n";
					break;
				}
				case GL_STACK_UNDERFLOW:
				{
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to underflow\n";
					break;
				}
				case GL_STACK_OVERFLOW:
				{
					std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to overflow\n";
					break;
				}
			}
		}
	}
}
