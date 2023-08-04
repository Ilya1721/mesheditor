#include "TestUtils.h"

namespace Vec
{
	TEST(VecConstructors, DefaultConstructor)
	{
		glm::vec2 vec2Uninit;
		EXPECT_TRUE(vec2Uninit != glm::vec2(0.0f, 0.0f));
		glm::vec2 vec2init(0.0f);
		EXPECT_TRUE(vec2init == glm::vec2(0.0f, 0.0f));
	}
}