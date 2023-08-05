#include "TestUtils.h"

#include "GeometryCore/Numeric.h"

namespace Numeric
{
	TEST(Functions, IsEqual)
	{
		EXPECT_TRUE(!isEqual(1.0f, 1.00001f));
		EXPECT_TRUE(isEqual(1.0f, 1.0f));
		EXPECT_TRUE(!isEqual(1.0f, -1.0f));
		EXPECT_TRUE(isEqual(0.0f, 0.0f));
		EXPECT_TRUE(isEqual(0.0f, -0.0f));
	}
}