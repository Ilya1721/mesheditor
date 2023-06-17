export module MeshFilesLoaderConsts;

import <string>;

export namespace MeshFilesLoader
{
	constexpr int STL_HEADER_SIZE = 80;
	const inline std::string STL_DELIMITERS = " ,\t\n";
	const inline std::string NORMAL_KEYWORD = "normal";
	const inline std::string VERTEX_KEYWORD = "vertex";
}
