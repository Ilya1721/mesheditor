#include "MeshFilesLoader.h"

#include <array>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"

#include "Constants.h"

namespace
{
	bool isBinaryFile(const std::string& fileContent)
	{
		if (fileContent.empty())
		{
			throw std::exception("File content is empty");
		}

		const char* buffer = fileContent.c_str();
		buffer += MeshFilesLoader::STL_HEADER_SIZE;

		auto numberOfTriangles = *reinterpret_cast<const uint32_t*>(buffer);
		auto correctBinaryFileSize = numberOfTriangles * 50 + 84;

		return correctBinaryFileSize == fileContent.size();
	}

	void readTokenAsVector(char*& currentToken, const char* delimiter, char*& nextToken, glm::vec3& coordinates)
	{
		for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
		{
			currentToken = strtok_s(nullptr, delimiter, &nextToken);
			coordinates[coordIdx] = std::stof(currentToken);
		}
	}

	void readCoordinatesFromBuffer(glm::vec3& coordinates, const char*& buffer)
	{
		for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
		{
			coordinates[coordIdx] = *reinterpret_cast<const float*>(buffer);
			buffer += sizeof(float);
		}
	}

	MeshCore::Mesh readText(std::string& fileContent)
	{
		auto contentString = fileContent.data();
		auto delimiters = MeshFilesLoader::STL_DELIMITERS.c_str();

		MeshCore::Mesh mesh;
		std::vector<MeshCore::Vertex> vertices;

		char* nextToken = nullptr;
		char* currentToken = strtok_s(contentString, delimiters, &nextToken);

		while (currentToken != nullptr)
		{
			if (Utility::isEqual(currentToken, MeshFilesLoader::KEYWORD_NORMAL))
			{
				vertices.emplace_back();
				readTokenAsVector(currentToken, delimiters, nextToken, vertices.back().normal);
			}
			else if (Utility::isEqual(currentToken, MeshFilesLoader::KEYWORD_VERTEX))
			{
				readTokenAsVector(currentToken, delimiters, nextToken, vertices.back().pos);
				mesh.addVertex(vertices.back());
			}

			currentToken = strtok_s(nullptr, delimiters, &nextToken);
		}

		return mesh;
	}

	MeshCore::Mesh readBinary(const std::string& fileContent)
	{
		const char* buffer = fileContent.c_str();
		buffer += MeshFilesLoader::STL_HEADER_SIZE;

		auto facesCount = *reinterpret_cast<const uint32_t*>(buffer);
		buffer += sizeof(uint32_t);

		MeshCore::Mesh mesh;

		for (size_t faceIdx = 0; faceIdx < facesCount; ++faceIdx)
		{
			glm::vec3 faceNormal{};
			readCoordinatesFromBuffer(faceNormal, buffer);

			for (int vertexIdx = 0; vertexIdx < 3; ++vertexIdx)
			{
				MeshCore::Vertex vertex{};
				vertex.normal = faceNormal;
				readCoordinatesFromBuffer(vertex.pos, buffer);
				mesh.addVertex(vertex);
			}

			buffer += sizeof(uint16_t);
		}

		return mesh;
	}
}

namespace MeshFilesLoader
{
	MeshCore::Mesh loadSTL(const std::filesystem::path& filePath)
	{
		if (!Utility::isEqual(filePath.extension().string(), ".stl"))
		{
			throw std::exception("This file is not of stl format");
		}

		auto fileContent = Utility::readFile(filePath);

		return isBinaryFile(fileContent) ? readBinary(fileContent) : readText(fileContent);
	}
}