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
		mRootObject(MeshCore::Object3D(nullptr, MeshFilesLoader::loadSTL(meshFilePath)))
	{
		init();
	}

	void Scene::init()
	{
		moveRootObjectToOrigin();
		initRenderBuffer();
		initLighting();
		initShaderTransformationSystem();
		adjustCameraAndLight();
	}

	void Scene::initRenderBuffer()
	{
		mRenderer.getRenderBuffer().setRenderData(mRootObject.getRenderData());
	}

	void Scene::initLighting()
	{
		auto& lighting = mRenderer.getLighting();
		lighting.setObjectColor(glm::value_ptr(OBJECT_COLOR));
		lighting.setLightColor(glm::value_ptr(LIGHT_COLOR));
		lighting.setAmbientStrength(AMBIENT_STRENGTH);
		lighting.setSpecularStrength(SPECULAR_STRENGTH);
		lighting.setShininess(LIGHT_SHININESS);
	}

	void Scene::initShaderTransformationSystem()
	{
		auto& shaderTransformationSystem = mRenderer.getShaderTransformationSystem();
		shaderTransformationSystem.setModel(glm::value_ptr(mRootObject.getTransform()));
		shaderTransformationSystem.setView(glm::value_ptr(getViewMatrix()));
		shaderTransformationSystem.setProjection(glm::value_ptr(mParentWindow->getViewport()->getProjectionMatrix()));
	}

	void Scene::render()
	{
		mRenderer.render();
	}

	void Scene::adjustCameraAndLight()
	{
		MeshCore::AABBox rootObjectBBox;
		rootObjectBBox.setFromObject(mRootObject);
		adjustCamera(rootObjectBBox, mParentWindow->getViewport()->getFov());
		adjustLightPos(rootObjectBBox);
	}

	void Scene::adjustLightPos(const MeshCore::AABBox& bbox)
	{
		auto cameraXYVec = (mCamera.getRight() + mCamera.getUp()) * bbox.getHeight() * 0.25f;
		auto cameraZVec = -mCamera.getNormalizedDirection() * LIGHT_SOURCE_TO_CAMERA_DISTANCE;
		auto lightPos = mCamera.getPosition() + cameraXYVec + cameraZVec;
		glm::vec3 lightPosInViewSpace = mCamera.getViewMatrix() * glm::vec4(lightPos, 1.0f);
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInViewSpace));
	}

	void Scene::adjustCamera(const MeshCore::AABBox& bbox, float fov)
	{
		mCamera.adjust(bbox, fov);
		mRenderer.getShaderTransformationSystem().setView(glm::value_ptr(mCamera.getViewMatrix()));
		glm::vec3 cameraPosInViewSpace = mCamera.getViewMatrix() * glm::vec4(mCamera.getPosition(), 1.0f);
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInViewSpace));
	}

	void Scene::pan(const glm::vec3& unProjectedStartPoint, const glm::vec3& unProjectedEndPoint)
	{
		mCamera.pan(unProjectedStartPoint, unProjectedEndPoint);
		mRenderer.getShaderTransformationSystem().setView(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::orbit(const glm::vec3& unProjectedStartPoint, const glm::vec3& unProjectedEndPoint)
	{
		mCamera.orbit(unProjectedStartPoint, unProjectedEndPoint);
		mRenderer.getShaderTransformationSystem().setView(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::zoomToPoint(const glm::vec3& unProjectedCursorPos, float yOffset)
	{
		MeshCore::AABBox rootObjectBBox;
		rootObjectBBox.setFromObject(mRootObject);
		mCamera.zoomToPoint(unProjectedCursorPos, yOffset * rootObjectBBox.getHeight() * ZOOM_STEP_KOEF);
		mRenderer.getShaderTransformationSystem().setView(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::setProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		mRenderer.getShaderTransformationSystem().setProjection(glm::value_ptr(projectionMatrix));
	}

	const glm::mat4& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}

	void Scene::moveRootObjectToOrigin()
	{
		MeshCore::AABBox rootObjectBBox;
		rootObjectBBox.setFromObject(mRootObject);
		mRootObject.setTransform(glm::translate(glm::mat4(1.0f), -rootObjectBBox.getCenter()));
	}
}
