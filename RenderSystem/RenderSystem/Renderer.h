#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"

#include "RenderBuffer.h"
#include "Constants.h"
#include "SceneDecoration.h"
#include "HighlightedFacesData.h"

namespace RenderSystem
{
	class Object3D;
	class SceneShaderProgram;

	using ObjectVertexMap = std::unordered_map<const Object3D*, int>;

	class Renderer
	{
	public:
		Renderer(SceneShaderProgram* sceneShaderProgram);
		Renderer(Renderer&&) = delete;

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
		void renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc);

	private:
		SceneShaderProgram* mSceneShaderProgram;
		RenderBuffer mModelRenderBuffer;
		RenderBuffer mDecorationsRenderBuffer;
	};
}

