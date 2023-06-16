#pragma once

#include <glm/glm.hpp>

namespace Geometry
{
	class Vector3D;

	using MatType = glm::mat<4, 4, double, glm::qualifier::defaultp>;

	class Mat4Impl
	{
	public:
		Mat4Impl() noexcept = default;
		Mat4Impl(double arg) noexcept;

		Mat4Impl(const Mat4Impl& other) noexcept;
		Mat4Impl(Mat4Impl&& other) noexcept;
		Mat4Impl& operator=(const Mat4Impl& other) noexcept;
		Mat4Impl& operator=(Mat4Impl&& other) noexcept;
		Mat4Impl(const MatType& matrix) noexcept;
		Mat4Impl(MatType&& matrix) noexcept;

		double* valuePtr() noexcept;
		const MatType& getImplMat() const noexcept;
		bool operator==(const Mat4Impl& other) const noexcept;
		Mat4Impl operator*(const Mat4Impl& other) const noexcept;

		static Mat4Impl lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp) noexcept;
		static Mat4Impl ortho(double left, double right, double bottom, double top, double zNear, double zFar) noexcept;
		static Mat4Impl perspective(double fov, double aspect, double zNear, double zFar) noexcept;

	private:
		MatType mImplMat;
	};
}
