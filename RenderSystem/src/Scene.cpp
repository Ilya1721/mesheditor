module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include "GeometryCore/Matrix.h"
module Scene;

import STLLoader;
import AABBox;
import Window;

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
		mRenderer.setRenderData(mRootObject.getRenderData());
		mRenderer.setModel(mRootObject.getTransform().valuePtr());
		mRenderer.setView(mCamera.getViewMatrix().valuePtr());
		mRenderer.setProjection(mParentWindow->getViewport()->getProjectionMatrix().valuePtr());
	}

	void Scene::render() noexcept
	{
		mRenderer.render();
	}

	void Scene::adjustCamera(float fov)
	{
		MeshCore::AABBox bbox;
		bbox.setFromObject(mRootObject);
		mCamera.adjust(bbox, fov);
	}
}
