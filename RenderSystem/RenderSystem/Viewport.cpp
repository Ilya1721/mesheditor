#include "Viewport.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MeshCore/Object3D.h"

#include "Constants.h"
#include "GlobalRenderState.h"
#include "Window.h"

namespace
{
	using namespace RenderSystem;

	GlobalRenderState* gGlobalRenderState = nullptr;
	Window* gWindow = nullptr;
}

namespace RenderSystem
{
	Viewport::Viewport() :
		mFov(FOV),
		mNearPlaneDistance(NEAR_PLANE_DISTANCE),
		mFarPlaneDistance(FAR_PLANE_DISTANCE),
		mPos(VIEWPORT_POSITION.x, VIEWPORT_POSITION.y),
		mWidth(),
		mHeight(),
		mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
		mProjectionMatrix(1.0f),
		mRootBBox(nullptr),
		mBBoxViewportGapCoef(BBOX_VIEWPORT_GAP_COEF)
	{
		gGlobalRenderState = &GlobalRenderState::getInstance();
		gWindow = &Window::getInstance();
		gGlobalRenderState->addRootObjectInitializedCallback([this]() {
			init(gWindow->getWidth(), gWindow->getHeight(), &gGlobalRenderState->getRootObject()->getBBox());
		});
	}

	Viewport& Viewport::getInstance()
	{
		static Viewport sInstance;
		return sInstance;
	}

	void Viewport::init(int width, int height, const MeshCore::AABBox* rootBBox)
	{
		mWidth = width;
		mHeight = height;
		mRootBBox = rootBBox;
		resize(width, height);
	}

	glm::mat4 Viewport::createProjectionMatrix() const
	{
		auto aspectRatio = mWidth / mHeight;

		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			float halfOrthoHeight = mRootBBox->getHeight() * 0.5f * mBBoxViewportGapCoef;
			float halfOrthoWidth = mRootBBox->getHeight() * aspectRatio * 0.5f * mBBoxViewportGapCoef;

			return glm::ortho(-halfOrthoWidth, halfOrthoWidth, -halfOrthoHeight, halfOrthoHeight, mNearPlaneDistance, mFarPlaneDistance);
		}

		return glm::perspective(glm::radians(mFov), aspectRatio, mNearPlaneDistance, mFarPlaneDistance);
	}

	void Viewport::invokeEditOperation(const std::function<void()>& action)
	{
		action();
		mProjectionMatrix = createProjectionMatrix();
		mViewportEditedCM.invokeCallbacks();
	}

	void Viewport::setProjectionType(PROJECTION_TYPE projectionType)
	{
		invokeEditOperation([this, &projectionType]() {
			mProjectionType = projectionType;
		});
	}

	void Viewport::resize(int width, int height)
	{
		invokeEditOperation([this, width, height]() {
			mWidth = width;
			mHeight = height;
			glViewport(mPos.x, mPos.y, mWidth, mHeight);
		});
	}

	void Viewport::zoom(float step)
	{
		invokeEditOperation([this, &step]() {
			mBBoxViewportGapCoef += step;
		});
	}

	void Viewport::addOnViewportEditedCallback(const std::function<void()>& callback)
	{
		mViewportEditedCM.addCallback(callback);
	}

	const glm::mat4& Viewport::getProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	const PROJECTION_TYPE Viewport::getProjectionType() const
	{
		return mProjectionType;
	}

	float Viewport::getFov() const
	{
		return mFov;
	}

	float Viewport::getNearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Viewport::getFarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}
		 
	float Viewport::getWidth() const
	{
		return mWidth;
	}

	float Viewport::getHeight() const
	{
		return mHeight;
	}

	const glm::ivec2& Viewport::getPos() const
	{
		return mPos;
	}
}
