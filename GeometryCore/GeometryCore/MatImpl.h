#pragma once

#include <glm/glm.hpp>

namespace Geometry
{
	class Vector3D;

	using MatType = glm::mat<4, 4, float, glm::qualifier::defaultp>;

	class Mat4Impl
	{
	public:
		Mat4Impl() = default;
		Mat4Impl(float arg);

		Mat4Impl(const Mat4Impl& other);
		Mat4Impl(Mat4Impl&& other) noexcept;
		Mat4Impl& operator=(const Mat4Impl& other);
		Mat4Impl& operator=(Mat4Impl&& other) noexcept;
		Mat4Impl(const MatType& matrix);
		Mat4Impl(MatType&& matrix) noexcept;

		float* valuePtr();
		const MatType& getImplMat() const;
		bool operator==(const Mat4Impl& other) const;
		Mat4Impl operator*(const Mat4Impl& other) const;

		static Mat4Impl lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp);
		static Mat4Impl ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		static Mat4Impl perspective(float fov, float aspect, float zNear, float zFar);

	private:
		MatType mImplMat;
	};
}
