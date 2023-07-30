#include "TestUtils.h"

#include "GeometryCore/Numeric.h"

namespace Methods
{
	TEST(Numeric, IsEqual)
	{
		EXPECT_TRUE(!isEqual(1.0f, 1.00001f));
		EXPECT_TRUE(isEqual(1.0f, 1.0f));
		EXPECT_TRUE(!isEqual(1.0f, -1.0f));
		EXPECT_TRUE(isEqual(0.0f, 0.0f));
		EXPECT_TRUE(isEqual(0.0f, -0.0f));
	}

	TEST(Numeric, ToRadians)
	{
		EXPECT_NEAR(toRadians(90.0f), HALF_PI, EPSILON);
		EXPECT_NEAR(toRadians(-90.0f), -HALF_PI, EPSILON);
	}

	TEST(Numeric, ToDegrees)
	{
		EXPECT_NEAR(toDegrees(HALF_PI), 90.0f, EPSILON);
		EXPECT_NEAR(toDegrees(-HALF_PI), -90.0f, EPSILON);
	}

	TEST(Numeric, Tan)
	{
		EXPECT_NEAR(Geometry::tan(45.0f), 1.619775f, EPSILON);
	}

	TEST(Numeric, Angle)
	{
		Vector3D xAxis(1.0f, 0.0f, 0.0f);
		Vector3D yAxis(0.0f, 1.0f, 0.0f);
		EXPECT_NEAR(angle(xAxis, yAxis), HALF_PI, EPSILON);
	}
}