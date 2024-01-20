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
		shaderTransformationSystem.setViewModel(glm::value_ptr(getViewMatrix() * mRootObject.getTransform()));
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
		auto lightPos = mCamera.getEye() + cameraXYVec + cameraZVec;
		glm::vec3 lightPosInCameraSpace = mCamera.getViewMatrix() * mRootObject.getTransform() * glm::vec4(lightPos, 1.0f);
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInCameraSpace));
	}

	void Scene::adjustCamera(const MeshCore::AABBox& bbox, float fov)
	{
		mCamera.adjust(bbox, fov);
		const auto modelView = mCamera.getViewMatrix() * mRootObject.getTransform();
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(modelView));
		glm::vec3 cameraPosInCameraSpace = modelView * glm::vec4(mCamera.getEye(), 1.0f);
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInCameraSpace));
	}

	void Scene::pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace)
	{
		mCamera.pan(startPointInWorldSpace, endPointInWorldSpace);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix() * mRootObject.getTransform()));
	}

	void Scene::orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC)
	{
		mCamera.orbit(startPointInNDC, endPointInNDC);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix() * mRootObject.getTransform()));
	}

	void Scene::zoom(float yOffset)
	{
		MeshCore::AABBox rootObjectBBox;
		rootObjectBBox.setFromObject(mRootObject);
		mCamera.zoom(yOffset * rootObjectBBox.getHeight() * ZOOM_STEP_KOEF);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix() * mRootObject.getTransform()));
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
