#include "Utility/FileHelper.h"

#include "TestUtils.h"

using namespace std::filesystem;

namespace FileHelper
{
	TEST(Functions, readFile)
	{
		auto testFilePath = current_path() / path(R"(..\..\Tests\TestFiles\ReadFileTest.txt)");
		auto fileContent = Utility::readFile(testFilePath);
		EXPECT_STREQ(fileContent.c_str(), "Hello World!\nHello World next line");
	}
}