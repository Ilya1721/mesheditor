module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
module Scene;

import STLLoader;
import AABBox;
import Window;
import RenderSystemConsts;

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Window* parentWindow) :
		mParentWindow(parentWindow),
		mRenderer(),
		mCamera(),
		mRootObject()
	{
		MeshFilesLoader::STLLoader stlLoader;
		mRootObject = MeshCore::Object3D(nullptr, stlLoader.loadMesh(meshFilePath));
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
		renderBuffer.setModel(getModelMatrix().valuePtr());
		renderBuffer.setView(getViewMatrix().valuePtr());
		renderBuffer.setProjection(getProjectionMatrix().valuePtr());
	}

	void Scene::initLighting()
	{
		auto& lighting = mRenderer.getLighting();
		lighting.setObjectColor(DEFAULT_OBJECT_COLOR.valuePtr());
		lighting.setAmbientColor(AMBIENT_COLOR.valuePtr());
		lighting.setDiffuseColor(DIFFUSE_COLOR.valuePtr());
		lighting.setAmbientStrength(AMBIENT_STRENGTH);
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
		const auto& max = bbox.getMax();
		auto lightPos = max + (max - bbox.getCenter()) * 2.0f;
		mRenderer.getLighting().setLightPos(lightPos.valuePtr());
	}

	void Scene::adjustCamera(const MeshCore::AABBox& bbox, float fov)
	{
		mCamera.adjust(bbox, fov);
		mRenderer.getRenderBuffer().setView(mCamera.getViewMatrix().valuePtr());
	}

	void Scene::pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint)
	{
		mCamera.pan(firstPoint, secondPoint);
		mRenderer.getRenderBuffer().setView(mCamera.getViewMatrix().valuePtr());
	}

	void Scene::setProjectionMatrix(const Geometry::Matrix4D& projectionMatrix)
	{
		mRenderer.getRenderBuffer().setProjection(projectionMatrix.valuePtr());
	}

	const Geometry::Matrix4D& Scene::getModelMatrix() const
	{
		return mRootObject.getTransform();
	}

	const Geometry::Matrix4D& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}

	const Geometry::Matrix4D& Scene::getProjectionMatrix() const
	{
		return mParentWindow->getViewport()->getProjectionMatrix();
	}
}
