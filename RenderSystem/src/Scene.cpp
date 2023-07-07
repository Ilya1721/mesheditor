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

using namespace Geometry;

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
		lighting.setLightColor(LIGHT_COLOR.valuePtr());
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
		auto lightPosInViewSpace = (mCamera.getViewMatrix() * Vector4D(lightPos, 1.0f)).getVec3();
		mRenderer.getLighting().setLightPos(lightPosInViewSpace.valuePtr());
	}

	void Scene::adjustCamera(const MeshCore::AABBox& bbox, float fov)
	{
		mCamera.adjust(bbox, fov);
		mRenderer.getRenderBuffer().setView(mCamera.getViewMatrix().valuePtr());
		auto cameraPosInViewSpace = (mCamera.getViewMatrix() * Vector4D(mCamera.getPosition(), 1.0f)).getVec3();
		mRenderer.getLighting().setCameraPos(cameraPosInViewSpace.valuePtr());
	}

	void Scene::pan(const Vector3D& firstPoint, const Vector3D& secondPoint)
	{
		mCamera.pan(firstPoint, secondPoint);
		mRenderer.getRenderBuffer().setView(mCamera.getViewMatrix().valuePtr());
	}

	void Scene::zoomToPoint(const Geometry::Vector3D& unProjectedMousePos, int scrollSign)
	{
		mCamera.zoomToPoint(unProjectedMousePos, scrollSign);
		mRenderer.getRenderBuffer().setView(mCamera.getViewMatrix().valuePtr());
	}

	void Scene::setProjectionMatrix(const Matrix4D& projectionMatrix)
	{
		mRenderer.getRenderBuffer().setProjection(projectionMatrix.valuePtr());
	}

	const Matrix4D& Scene::getModelMatrix() const
	{
		return mRootObject.getTransform();
	}

	const Matrix4D& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}

	const Matrix4D& Scene::getProjectionMatrix() const
	{
		return mParentWindow->getViewport()->getProjectionMatrix();
	}
}
