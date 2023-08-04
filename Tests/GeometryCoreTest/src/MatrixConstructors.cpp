#include "TestUtils.h"

namespace Mat
{
	TEST(MatConstructors, DefaultConstructor)
	{
		glm::mat4 matrixUninit;
		EXPECT_FALSE(matrixEqualMatArray(matrixUninit, identityMatrix));
		glm::mat4 matrixInit(1.0f);
		EXPECT_TRUE(matrixEqualMatArray(matrixInit, identityMatrix));
	}

	TEST(MatConstructors, OneValueConstructor)
	{
		glm::mat4 matrix(scaleFactor);
		EXPECT_TRUE(matrixEqualMatArray(matrix, scaleMatrix));
	}
}