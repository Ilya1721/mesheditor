#include "pch.h"

#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif

#include "glad.h"

#include "MeshFilesLoader/STLLoader.h"

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, std::unique_ptr<BasicRenderer> renderer) noexcept :
		mRenderer(std::move(renderer)), mVBO(0), mVAO(0), mViewport(0, 0, 800, 600)
	{
		MeshFilesLoader::STLLoader stlFileLoader;
		auto mesh = std::move(stlFileLoader.loadMesh(meshFilePath));
	}

	Scene::~Scene() noexcept
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mVAO);
	}

	void Scene::init() noexcept
	{
		mRenderer->init();

		glClearColor(0.09f, 0.42f, 0.69f, 1.0f);

		glViewport(mViewport.getX(), mViewport.getY(), mViewport.getWidth(), mViewport.getHeight());

		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		mRenderer->printOpenGLErrorMessage();

		mViewport.setFarPlaneDistance(1000.0f);
		mViewport.setNearPlaneDistance(0.1f);
		mViewport.getCamera().setPositionTargetUp({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });


	}

	void Scene::setViewport(int x, int y, int width, int height) noexcept
	{
		mViewport.setViewport(x, y, width, height);
	}

	void Scene::setRenderer(std::unique_ptr<BasicRenderer> renderer) noexcept
	{
		mRenderer = std::move(renderer);
	}

	/*void Scene::setMesh(const MeshCore::Mesh& mesh) noexcept
	{
		mMeshData.mesh = mesh;
	}*/

	void Scene::render() noexcept
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if (mMeshData.needUpdate)
		{
			loadData(mMeshData.mesh.getVertices());
			mMeshData.needUpdate = false;
		}

		auto mvp = mViewport.calcProjectionMatrix(PROJECTION_TYPE::PERSPECTIVE) * mViewport.getCamera().calcViewMatrix();
		mRenderer->setMVP(glm::value_ptr(mvp));

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
		 
	void Scene::loadData(const std::vector<MeshCore::Vertex>& vertices) noexcept
	{
		/*mPositions.reserve(vertices.size() * Constants::COORDINATES_PER_VERTEX);
		mNormals.reserve(vertices.size() * Constants::COORDINATES_PER_NORMAL);
		mColors.reserve(vertices.size() * Constants::COLOR_COMPONENTS_COUNT);

		static_assert(Constants::COORDINATES_PER_VERTEX == Constants::COORDINATES_PER_NORMAL &&
			Constants::COORDINATES_PER_VERTEX == Constants::COLOR_COMPONENTS_COUNT);

		for (const auto& vertex : vertices)
		{
			for (glm::vec3::length_type coordIdx = 0; coordIdx < Constants::COORDINATES_PER_VERTEX; ++coordIdx)
			{
				mPositions.push_back(vertex.pos[coordIdx]);
				mNormals.push_back(vertex.normal[coordIdx]);
				mColors.push_back(vertex.color[coordIdx]);
			}
		}*/

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
		glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float), mPositions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		mRenderer->printOpenGLErrorMessage();
	}
}
