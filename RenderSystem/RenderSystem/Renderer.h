#pragma once

#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"
#include "Utility/CallbackMechanism.h"

#include "RenderBuffer.h"
#include "Constants.h"
#include "SceneShaderProgram.h"
#include "DepthShaderProgram.h"

using namespace GeometryCore;

namespace MeshCore
{
	class Object3D;
}

namespace RenderSystem
{
	struct RenderPrimitive;

	class Renderer
	{
	public:
		static Renderer& getInstance();

		void render();
		void addInitializedCallback(const std::function<void()>& callback);
		void adjustLightPos();

		RenderBuffer& getRenderBuffer();
		SceneShaderProgram& getSceneShaderProgram();

	private:
		Renderer();
		~Renderer();

		void init();
		void initExtraFBO();
		void registerCallbacks();
		void renderExtra();
		void renderShadows();
		void renderHighlightedFaces();
		void renderWireframe();
		void renderWholeObjectHighlighted();
		void renderScene();
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);
		void renderExtraFBO(const std::function<void()>& renderFunc) const;

	private:
		unsigned int mExtraFBO;
		RenderBuffer mRenderBuffer;
		RenderBuffer mExtraRenderBuffer;
		SceneShaderProgram mSceneShaderProgram;
		DepthShaderProgram mDepthShaderProgram;
		Utility::CallbackMechanism mRendererInitCM;
	};
}

