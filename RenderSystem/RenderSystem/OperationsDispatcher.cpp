#include "OperationsDispatcher.h"

#include <glfw/glfw3.h>

#include "SurfaceHighlighter.h"
#include "SurfaceExtruder.h"
#include "WireframeRenderer.h"
#include "Orbit.h"
#include "Pan.h"
#include "Zoom.h"
#include "Picker.h"
#include "Mover.h"
#include "Cloner.h"

namespace RenderSystem
{
    OperationsDispatcher::OperationsDispatcher(Scene* scene) :
        mScene(scene)
    {
        initOperations();
    }

    void OperationsDispatcher::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseMove(startCursorPos, endCursorPos);
        }
    }

    void OperationsDispatcher::onMouseScroll(double offset)
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseScroll(offset);
        }
    }

    void OperationsDispatcher::onMouseClick()
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseClick();
        }
    }

    void OperationsDispatcher::onKeyPressed(int key)
    {
        for (auto& operation : mOperations)
        {
            operation->onKeyPressed(key);
        }
    }

    void OperationsDispatcher::initOperations()
    {
        addOperation<SurfaceHighlighter>();
        addOperation<SurfaceExtruder>();
        addOperation<WireframeRenderer>();
        addOperation<Orbit>();
        addOperation<Pan>();
        addOperation<Zoom>();
        addOperation<Picker>();
        addOperation<Mover>();
        addOperation<Cloner>();
    }
}