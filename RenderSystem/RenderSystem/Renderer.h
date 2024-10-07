#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"

#include "Lighting.h"
#include "RenderBuffer.h"
#include "ShaderTransformationSystem.h"
#include "Constants.h"
#include "SceneDecoration.h"
#include "HighlightedFacesData.h"

namespace RenderSystem
{
	class Object3D;

	using ObjectVertexMap = std::unordered_map<const Object3D*, int>;

	class Renderer
	{
	public:
		Renderer();
		Renderer(Renderer&&) = delete;
		~Renderer();

		void setModel(const float* model) const;
		void setView(const float* view) const;
		void setProjection(const float* projection) const;
		void setLightPos(const float* pos) const;
		void setCameraPos(const float* pos) const;

		void renderScene(const ObjectVertexMap& objectVertexOffsetMap);
		void renderSceneDecorations(const std::vector<SceneDecoration>& sceneDecorations, const glm::mat4& rootObjectTransform);
		void renderHighlightedFaces(const ObjectVertexMap& objectVertexOffsetMap, const HighlightedFacesData& highlightedFacesData);
		void renderWireframe(int sceneVertexCount, bool ifToRender);
		void renderWholeObjectHighlighted(const ObjectVertexMap& objectVertexOffsetMap, const Object3D* highlightedObject);
		void loadModelRenderData(const RenderData& renderData);
		void loadDecorationsRenderData(const RenderData& renderData);
		void cleanScreen();

	private:
		void init();
		void initShaders();
		void initShaderProgram();
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);

	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		ShaderTransformationSystem mShaderTransformationSystem;
		Lighting mLighting;
		RenderBuffer mModelRenderBuffer;
		RenderBuffer mDecorationsRenderBuffer;
	};
}

