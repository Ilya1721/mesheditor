#pragma once

#include <string>

namespace MeshFilesLoader
{
	constexpr int STL_HEADER_SIZE = 80;
	const inline std::string STL_DELIMITERS = " ,\t\n";
	const inline std::string KEYWORD_NORMAL = "normal";
	const inline std::string KEYWORD_VERTEX = "vertex";
}
