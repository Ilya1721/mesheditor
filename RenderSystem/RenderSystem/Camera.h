#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
	class Camera
	{
	public:
		Camera() noexcept;

		glm::mat4 calcViewMatrix() noexcept;

		void setTarget(const glm::vec3& newTarget) noexcept;
		const glm::vec3& getTarget() const noexcept;

		void setPosition(const glm::vec3& newPosition) noexcept;
		const glm::vec3& getPosition() const noexcept;

		void setUp(const glm::vec3& newUp) noexcept;
		const glm::vec3& getUp() const noexcept;

		void setPositionTargetUp(const glm::vec3& newPosition, const glm::vec3& newTarget, const glm::vec3& newUp) noexcept;

		void translate(const glm::vec3& movement) noexcept;

	private:
		void recalcCamera() noexcept;

	private:
		glm::vec3 mTarget;
		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mUp;
		glm::vec3 mRight;
	};
}

