#include "Transforms.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>

namespace GeometryCore
{
    glm::mat4 getRotationTransform(float rotationAngle, const glm::vec3& rotationAxis)
    {
        if (glm::all(glm::epsilonEqual(rotationAxis, glm::vec3(0.0f, 0.0f, 0.0f), 1e-6f)))
        {
            return glm::mat4(1.0f);
        }

        return glm::rotate(rotationAngle, rotationAxis);
    }

    glm::mat4 getTranslationTransform(const glm::vec3& destinationPoint, const glm::vec3& sourcePoint)
    {
        if (glm::all(glm::epsilonEqual(destinationPoint, sourcePoint, 1e-6f)))
        {
            return glm::mat4(1.0f);
        }

        return glm::translate(destinationPoint - sourcePoint);
    }
}