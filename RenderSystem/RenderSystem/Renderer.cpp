#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Mesh.h"

#include "Object3D.h"
#include "SceneShaderProgram.h"

using namespace MeshCore;

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;
}

namespace RenderSystem
{
	Renderer::Renderer(SceneShaderProgram* sceneShaderProgram) :
		mSceneShaderProgram(sceneShaderProgram),
		mModelRenderBuffer(),
		mDecorationsRenderBuffer()
	{
		init();
	}

	void Renderer::init()
	{
		mModelRenderBuffer.bind();
	}

	void Renderer::renderHighlightedFaces(const ObjectVertexMap& objectVertexOffsetMap, const HighlightedFacesData& highlightedFacesData)
	{
		renderOverlayPrimitives(!highlightedFacesData.facesIndices.empty(), HIGHLIGHT_MATERIAL,
		[&highlightedFacesData, &objectVertexOffsetMap, this]() {
			for (const auto& faceIdx : highlightedFacesData.facesIndices)
			{
				mSceneShaderProgram->setModel(highlightedFacesData.parentObject->getTransform());
				glDrawArrays(GL_TRIANGLES, faceIdx * 3 + objectVertexOffsetMap.at(highlightedFacesData.parentObject), 3);
			}
		});
	}

	void Renderer::renderWireframe(int sceneVertexCount, bool ifToRender)
	{
		renderOverlayPrimitives(ifToRender, WIREFRAME_MATERIAL, [&sceneVertexCount]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, sceneVertexCount);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderWholeObjectHighlighted(const ObjectVertexMap& objectVertexOffsetMap, const Object3D* highlightedObject)
	{
		const auto& highlightedObjectIt = objectVertexOffsetMap.find(highlightedObject);
		renderOverlayPrimitives(highlightedObjectIt != objectVertexOffsetMap.end(), HIGHLIGHT_MATERIAL, [this, highlightedObjectIt]() {
			auto& [object, vertexCount] = *highlightedObjectIt;
			mSceneShaderProgram->setModel(object->getTransform());
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getVertexCount());
		});
	}

	void Renderer::renderScene(const ObjectVertexMap& objectVertexOffsetMap)
	{
		for (auto& [object, vertexOffset] : objectVertexOffsetMap)
		{
			mSceneShaderProgram->setModel(object->getTransform());
			glDrawArrays(GL_TRIANGLES, vertexOffset, object->getVertexCount());
		}
	}

	void Renderer::renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
	{
		if (!renderCondition)
		{
			return;
		}

		mSceneShaderProgram->setMaterial(material);
		glDepthFunc(GL_LEQUAL);
		renderFunc();
		glDepthFunc(GL_LESS);
		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
	}

	void Renderer::cleanScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::loadDecorationsRenderData(const RenderData& renderData)
	{
		mDecorationsRenderBuffer.bind();
		mDecorationsRenderBuffer.loadRenderData(renderData);
		mModelRenderBuffer.bind();
	}

	void Renderer::loadModelRenderData(const RenderData& renderData)
	{
		mModelRenderBuffer.loadRenderData(renderData);
	}

	void Renderer::renderSceneDecorations(const std::vector<SceneDecoration>& sceneDecorations, const glm::mat4& rootObjectTransform)
	{
		mDecorationsRenderBuffer.bind();
		mSceneShaderProgram->setModel(glm::mat4(1.0f));

		int startIndex = 0;
		for (const auto& sceneDecoration : sceneDecorations)
		{
			auto vertexCount = sceneDecoration.renderData.getVertexCount();
			mSceneShaderProgram->setMaterial(sceneDecoration.material);
			glDrawArrays(sceneDecoration.renderMode, startIndex, vertexCount);
			startIndex += vertexCount;
		}

		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
		mSceneShaderProgram->setModel(rootObjectTransform);
		mModelRenderBuffer.bind();
	}
}
