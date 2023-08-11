#include "Utility/StringHelper.h"

#include "TestUtils.h"

namespace StringHelper
{
	TEST(Functions, IsEqual)
	{
		std::string first = "abc";
		std::string second = "abc";
		EXPECT_TRUE(Utility::isEqual(first, second));

		first = "ABC";
		EXPECT_TRUE(Utility::isEqual(first, second));
		EXPECT_FALSE(Utility::isEqual(first, second, true));

		first = "abv";
		EXPECT_FALSE(Utility::isEqual(first, second));

		first = "qqw";
		EXPECT_FALSE(Utility::isEqual(first, second));
	}
}