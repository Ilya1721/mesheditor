#include "LightSource.h"

#include "SceneShaderProgram.h"
#include "ShadowController.h"

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
        mShadowController->calcLightSourceSpace(pos);
    }
}
