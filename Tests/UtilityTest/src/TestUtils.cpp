#include "TestUtils.h"

#include <iostream>

void logStringComparison(const std::string& result, const std::string& expected)
{
	std::cout << "Result = " << result << std::endl;
	std::cout << "Expected = " << expected << std::endl;
}
