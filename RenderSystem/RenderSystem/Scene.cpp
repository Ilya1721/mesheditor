#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/type_ptr.hpp>

#include "MeshFilesLoader/MeshFilesLoader.h"
#include "MeshCore/AABBox.h"

#include "Window.h"
#include "Constants.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Window* parentWindow) :
		mParentWindow(parentWindow),
		mRenderer(),
		mCamera(),
		mRootObject()
	{
		mRootObject = MeshCore::Object3D(nullptr, MeshFilesLoader::loadSTL(meshFilePath));
		init();
	}

	void Scene::init()
	{
		initRenderBuffer();
		initLighting();
	}

	void Scene::initRenderBuffer()
	{
		auto& renderBuffer = mRenderer.getRenderBuffer();
		renderBuffer.setRenderData(mRootObject.getRenderData());
		renderBuffer.setModel(glm::value_ptr(getModelMatrix()));
		renderBuffer.setView(glm::value_ptr(getViewMatrix()));
		renderBuffer.setProjection(glm::value_ptr(getProjectionMatrix()));
	}

	void Scene::initLighting()
	{
		auto& lighting = mRenderer.getLighting();
		lighting.setObjectColor(glm::value_ptr(DEFAULT_OBJECT_COLOR));
		lighting.setLightColor(glm::value_ptr(LIGHT_COLOR));
		lighting.setAmbientStrength(AMBIENT_STRENGTH);
		lighting.setSpecularStrength(SPECULAR_STRENGTH);
		lighting.setShininess(LIGHT_SHININESS);
	}

	void Scene::render()
	{
		mRenderer.render();
	}

	void Scene::adjust(float fov)
	{
		MeshCore::AABBox bbox;
		bbox.setFromObject(mRootObject);
		adjustCamera(bbox, fov);
		adjustLightPos(bbox);
	}

	void Scene::adjustLightPos(const MeshCore::AABBox& bbox)
	{
		auto firstLightPosDirVec = (mCamera.getRight() + mCamera.getUp()) * bbox.getHeight();
		auto secondLightPosDirVec = -mCamera.getNormalizedDirection() * LIGHT_TO_CAMERA_DISTANCE;
		auto lightPos = mCamera.getPosition() + firstLightPosDirVec + secondLightPosDirVec;
		auto lightPosInViewSpace = glm::vec3(mCamera.getViewMatrix() * glm::vec4(lightPos, 1.0f));
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInViewSpace));
	}

	void Scene::adjustCamera(const MeshCore::AABBox& bbox, float fov)
	{
		mCamera.adjust(bbox, fov);
		mRenderer.getRenderBuffer().setView(glm::value_ptr(mCamera.getViewMatrix()));
		auto cameraPosInViewSpace = glm::vec3(mCamera.getViewMatrix() * glm::vec4(mCamera.getPosition(), 1.0f));
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInViewSpace));
	}

	void Scene::pan(const glm::vec3& firstPoint, const glm::vec3& secondPoint)
	{
		mCamera.pan(firstPoint, secondPoint);
		mRenderer.getRenderBuffer().setView(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::zoomToPoint(const glm::vec3& unProjectedMousePos, int scrollSign)
	{
		mCamera.zoomToPoint(unProjectedMousePos, scrollSign);
		mRenderer.getRenderBuffer().setView(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::setProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		mRenderer.getRenderBuffer().setProjection(glm::value_ptr(projectionMatrix));
	}

	const glm::mat4& Scene::getModelMatrix() const
	{
		return mRootObject.getTransform();
	}

	const glm::mat4& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}

	const glm::mat4& Scene::getProjectionMatrix() const
	{
		return mParentWindow->getViewport()->getProjectionMatrix();
	}
}
