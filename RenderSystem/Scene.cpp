#include "pch.h"

#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif

#include "glad.h"

#include "MeshFilesLoader/STLLoader.h"
#include "MeshCore/AABBox.h"

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, std::unique_ptr<BasicRenderer> renderer) noexcept :
		mRenderer(std::move(renderer)),
		mVBO(0),
		mVAO(0),
		mViewport(0, 0, 0, 0),
		mPanHandler(&mViewport),
		mSavedMousePos()
	{
		MeshFilesLoader::STLLoader stlFileLoader;
		auto mesh = std::make_unique<MeshCore::Mesh>(stlFileLoader.loadMesh(meshFilePath));
		mRootObject = std::make_unique<MeshCore::Object3D>(nullptr, std::move(mesh));
		mRootObject->prepareRenderData();
	}

	Scene::~Scene() noexcept
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteVertexArrays(1, &mVAO);
	}

	void Scene::initRenderer() noexcept
	{
		mRenderer->init();
		setBackgroundColor();

		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);
		glEnable(GL_DEPTH_TEST);

		mRenderer->printOpenGLErrorMessage();
	}

	void Scene::initViewport() noexcept
	{
		const auto& viewportPos = mViewport.getPos();
		glViewport(viewportPos.x, viewportPos.y, mViewport.getWidth(), mViewport.getHeight());

		mViewport.setFarPlaneDistance(1000.0f);
		mViewport.setNearPlaneDistance(0.1f);
		mViewport.calcProjectionMatrix();
	}

	void Scene::init() noexcept
	{
		initRenderer();
		initViewport();
		mViewport.adjustToObject(*mRootObject);
	}

	void Scene::setViewport(int x, int y, int width, int height) noexcept
	{
		mViewport.setViewport(x, y, width, height);
	}

	void Scene::setRenderer(std::unique_ptr<BasicRenderer> renderer) noexcept
	{
		mRenderer = std::move(renderer);
	}

	void Scene::setBackgroundColor(const glm::vec4& backgroundColor) noexcept
	{
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	}

	void Scene::render() noexcept
	{
		render(*mRootObject);
	}

	void Scene::setRenderData(const MeshCore::RenderData& renderData) noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
		glBufferData(GL_ARRAY_BUFFER, renderData.positions.size() * sizeof(float), renderData.positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		mRenderer->printOpenGLErrorMessage();
	}

	void Scene::render(const MeshCore::Object3D& object) noexcept
	{
		setRenderData(object.getRenderData());

		auto mvp = mViewport.getProjectionMatrix() * mViewport.getCamera().getViewMatrix();
		mRenderer->setMVP(glm::value_ptr(mvp));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		for (auto& object : object.getChildren())
		{
			render(object);
		}
	}

	void Scene::onMouseMove(const glm::vec2& mousePos, const MouseButtonsState& mouseButtonsState) noexcept
	{
		if (mouseButtonsState.middleButtonPressed)
		{
			mPanHandler.pan(mSavedMousePos, mousePos);
		}
	}

	void Scene::onMouseDown(int button, const glm::vec2& mousePos) noexcept
	{
		mSavedMousePos = mousePos;
	}

	void Scene::onMouseUp(int button) noexcept
	{
		//std::cerr << "Scene::onMouseUp, button = " << button << std::endl;
	}

	void Scene::onMouseScroll(double yOffset) noexcept
	{
		//std::cerr << "Scene::onMouseScroll, yOffset = " << yOffset << std::endl;
	}

	void Scene::onKeyDown(int keyCode) noexcept
	{
		//std::cerr << "Scene::onKeyDown, keyCode = " << keyCode << std::endl;
	}

	void Scene::onKeyUp(int keyCode) noexcept
	{
		//std::cerr << "Scene::onKeyUp, keyCode = " << keyCode << std::endl;
	}
}
