#include "Line.h"

#include <glm/gtx/vector_angle.hpp>

#include "Transforms.h"

namespace GeometryCore
{
    glm::mat4 Line::getTransformToSelf(const Line& source) const
    {
        auto lineDir = end - start;
        auto sourceLineDir = source.end - source.start;
        auto rotationAxis = glm::cross(lineDir, sourceLineDir);
        auto rotationAngle = glm::angle(glm::normalize(lineDir), glm::normalize(sourceLineDir));

        auto rotationTransform = getRotationTransform(rotationAngle, rotationAxis);
        auto translationTransform = getTranslationTransform(start, source.start);

        return translationTransform * rotationTransform;
    }
}