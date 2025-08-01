#include "LightSource.h"

#include "SceneShaderProgram.h"
#include "ShadowController.h"
#include <glm/ext/matrix_transform.hpp>

namespace RenderSystem
{
    LightSource::LightSource(SceneShaderProgram* sceneShaderProgram, ShadowController* shadowController) :
        mPos(),
        mSceneShaderProgram(sceneShaderProgram),
        mShadowController(shadowController)
    {}

    const Point3D& LightSource::getPosition() const
    {
        return mPos;
    }

    void LightSource::setPosition(const Point3D& pos)
    {
        mPos = pos;
        mSceneShaderProgram->setLightSourcePos(pos);

        const auto upVector = glm::normalize(glm::cross(pos, Vector3D(1.0, 0.0, 0.0)));
        const auto& lightViewMatrix = glm::lookAt(pos, Point3D(0.0f, 0.0f, 0.0f), upVector);
        mShadowController->setLightView(lightViewMatrix);
        mSceneShaderProgram->setLightView(lightViewMatrix);
    }
}
