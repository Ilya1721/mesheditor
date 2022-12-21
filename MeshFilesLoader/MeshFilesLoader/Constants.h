#pragma once

namespace MeshFilesLoader
{
	class Constants final
	{
	public:
		Constants() = delete;

		static const int STL_HEADER_SIZE = 80;
		static const inline std::string STL_DELIMITERS = " ,\t\n";
		static const inline std::string NORMAL_KEYWORD = "normal";
		static const inline std::string VERTEX_KEYWORD = "vertex";
	};
}
