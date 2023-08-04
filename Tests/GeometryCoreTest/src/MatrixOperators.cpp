#include "TestUtils.h"

namespace Mat
{
	TEST(Operators, MatMultVec)
	{
		glm::mat4 scaleMatrix(scaleFactor);
		glm::vec4 vec4(1.0f, 1.0f, 1.0f, 1.0f);
		auto result = scaleMatrix * vec4;
		EXPECT_TRUE(result == glm::vec4(2.0f, 2.0f, 2.0f, 2.0f));
	}
}