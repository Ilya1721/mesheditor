#include "pch.h"

#include "STLLoader.h"

#include "Utility/EqualityUtils.h"
#include "Utility/FileUtils.h"
#include "Constants.h"

namespace
{
	bool isBinaryFile(const std::string& fileContent)
	{
		if (fileContent.empty())
		{
			throw std::exception("File path is empty");
		}

		const char* buffer = fileContent.c_str();
		buffer += MeshFilesLoader::Constants::STL_HEADER_SIZE;

		auto numberOfTriangles = *reinterpret_cast<const uint32_t*>(buffer);
		
		auto correctBinaryFileSize = numberOfTriangles * 12llu * sizeof(float) + MeshFilesLoader::Constants::STL_HEADER_SIZE + sizeof(uint16_t);

		return correctBinaryFileSize == fileContent.size();
	}

	MeshCore::Mesh readText(std::string& fileContent)
	{
		auto charStr = fileContent.data();
		auto delimetersCharStr = MeshFilesLoader::Constants::STL_DELIMITERS.c_str();

		char* nextToken = nullptr;
		char* currentToken = strtok_s(charStr, delimetersCharStr, &nextToken);

		MeshCore::Mesh mesh;
		std::vector<MeshCore::Vertex> vertices;

		while (currentToken != nullptr)
		{
			bool newVertex = false;

			if (Utility::EqualityUtils::stringEquals(currentToken, MeshFilesLoader::Constants::NORMAL_KEYWORD))
			{
				vertices.emplace_back();

				for (glm::vec3::length_type coordIdx = 0; coordIdx < 3; ++coordIdx)
				{
					currentToken = strtok_s(nullptr, delimetersCharStr, &nextToken);
					vertices.back().normal[coordIdx] = std::stof(currentToken);
				}
			}
			else if (Utility::EqualityUtils::stringEquals(currentToken, MeshFilesLoader::Constants::VERTEX_KEYWORD))
			{
				for (glm::vec3::length_type coordIdx = 0; coordIdx < 3; ++coordIdx)
				{
					currentToken = strtok_s(nullptr, delimetersCharStr, &nextToken);
					vertices.back().pos[coordIdx] = std::stof(currentToken);
				}

				newVertex = true;
			}

			if (newVertex)
			{
				mesh.addVertex(vertices.back());
			}

			currentToken = strtok_s(nullptr, delimetersCharStr, &nextToken);
		}

		return mesh;
	}

	MeshCore::Mesh readBinary(const std::string& fileContent)
	{
		const char* buffer = fileContent.c_str();
		buffer += MeshFilesLoader::Constants::STL_HEADER_SIZE;

		uint32_t numberOfTriangles = *reinterpret_cast<const uint32_t*>(buffer);
		buffer += sizeof(uint32_t);

		MeshCore::Mesh mesh;

		for (size_t idx = 0; idx < 3llu * numberOfTriangles; ++idx)
		{
			MeshCore::Vertex vertex;
			
			for (glm::vec3::length_type coordIdx = 0; coordIdx < 3; ++coordIdx)
			{
				vertex.normal[coordIdx] = *reinterpret_cast<const float*>(buffer);
				buffer += sizeof(float);
			}

			for (glm::vec3::length_type coordIdx = 0; coordIdx < 3; ++coordIdx)
			{
				vertex.pos[coordIdx] = *reinterpret_cast<const float*>(buffer);
				buffer += sizeof(float);
			}

			mesh.addVertex(vertex);
		}

		return mesh;
	}
}

namespace MeshFilesLoader
{
	MeshCore::Mesh STLLoader::loadMesh(const std::filesystem::path& filePath) const noexcept
	{
		try
		{
			if (!Utility::EqualityUtils::stringEquals(filePath.extension().string(), ".stl"))
			{
				throw std::exception("This file is not of stl format");
			}

			auto fileContent = Utility::FileUtils::readFile(filePath);

			if (isBinaryFile(fileContent))
			{
				return readBinary(fileContent);
			}
			else
			{
				return readText(fileContent);
			}
		}
		catch (std::exception& exc)
		{
			std::cerr << exc.what() << std::endl;
		}

		return MeshCore::Mesh();
	}
}
