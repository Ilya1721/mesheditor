#include "TestUtils.h"

#include "GeometryCore/Matrix.h"

using namespace Geometry;

namespace Methods
{
	TEST(Vector2D, Setters)
	{
		Vector2D vec;
		vec.setX(testX);
		vec.setY(testY);
		EXPECT_TRUE(vec == (Vector2D{ testX, testY }));
		vec.setR(5.0f);
		vec.setG(6.0f);
		EXPECT_TRUE(vec == (Vector2D{ 5.0f, 6.0f }));
	}

	TEST(Vector3D, Setters)
	{
		Vector3D vec;
		vec.setZ(testZ);
		EXPECT_FLOAT_EQ(vec.z(), testZ);
		vec.setB(5.0f);
		EXPECT_FLOAT_EQ(vec.b(), 5.0f);
	}

	TEST(Vector4D, Setters)
	{
		Vector4D vec;
		vec.setW(testW);
		EXPECT_FLOAT_EQ(vec.w(), testW);
		vec.setA(5.0f);
		EXPECT_FLOAT_EQ(vec.a(), 5.0f);
	}

	TEST(Vector2D, Length)
	{
		Vector2D vec(3.0f, 4.0f);
		EXPECT_FLOAT_EQ(vec.length(), 5.0f);
	}

	TEST(Vector2D, GetNormalized)
	{
		Vector2D vec(3.0f, 4.0f);
		auto normalizedVec = vec.getNormalized();
		EXPECT_TRUE(normalizedVec == (Vector2D{ 0.6f, 0.8f }));
	}

	TEST(Vector3D, GetNormalized)
	{
		Vector3D vec(2.0f, 3.0f, 6.0f);
		auto normalizedVec = vec.getNormalized();
		EXPECT_TRUE(normalizedVec == (Vector3D{ 2.0f / vec.length(), 3.0f / vec.length(), 6.0f / vec.length() }));
	}

	TEST(Vector4D, GetNormalized)
	{
		Vector4D vec(2.0f, 3.0f, 6.0f, 8.0f);
		auto normalizedVec = vec.getNormalized();
		EXPECT_TRUE(normalizedVec == (Vector4D{ 2.0f / vec.length(), 3.0f / vec.length(), 6.0f / vec.length(), 8.0f / vec.length() }));
	}

	TEST(Vector4D, GetVec3)
	{
		Vector4D vec(2.0f, 3.0f, 6.0f, 8.0f);
		auto vec3 = vec.getVec3();
		EXPECT_TRUE(vec3 == (Vector3D{ 2.0f, 3.0f, 6.0f }));
	}

	TEST(Vector3D, CrossProduct)
	{
		Vector3D vec(2.0f, 3.0f, 6.0f);
		Vector3D otherVec(1.0f, 2.0f, 3.0f);
		auto result = vec.cross(otherVec);
		EXPECT_TRUE(result == (Vector3D{ -3.0f, 0.0f, 1.0f }));
	}

	namespace CursorPoint
	{
		constexpr float screenWidth = 1920.0f;
		constexpr float screenHeight = 1080.0f;
		constexpr float screenAspect = screenWidth / screenHeight;

		const Vector4D viewport = Vector4D(0.0f, 0.0f, screenWidth, screenHeight);
		const Matrix4D model = Matrix4D(1.0f);
		const Vector3D cameraPos = Vector3D(0.0f, 0.0f, 5.0f);
		const Matrix4D projection = Matrix4D::perspective(45.0f, screenAspect, 0.01f, 1000.0f);
		const Matrix4D view = Matrix4D::lookAt(cameraPos, Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
		const Matrix4D modelView = view;

		TEST(Vector3D, Project)
		{
			Vector4D point4D(1.0f, 2.0f, 3.0f, 1.0f);

			auto pointInView = modelView * point4D;
			auto pointInClip = projection * pointInView;
			auto normalizedPoint = Vector3D(pointInClip) / pointInClip.w();
			auto screenX = (normalizedPoint.x() * 0.5f + 0.5f) * screenWidth;
			auto screenY = (normalizedPoint.y() * 0.5f + 0.5f) * screenHeight;
			auto z = normalizedPoint.z() * 0.5f + 0.5f;

			auto result = point4D.getVec3().project(modelView, projection, viewport);
			EXPECT_FLOAT_EQ(result.x(), screenX);
			EXPECT_FLOAT_EQ(result.y(), screenY);
			EXPECT_FLOAT_EQ(z, result.z());
		}

		TEST(Vector3D, UnProject)
		{
			Vector3D cursorPoint(200.0f, 400.0f, 0.0f);
			auto xNDC = 2.0f * cursorPoint.x() / screenWidth - 1.0f;
			auto yNDC = 2.0f * cursorPoint.y() / screenHeight - 1.0f;
			auto zNDC = 2.0f * cursorPoint.z() - 1.0f;
			Vector4D point4D(xNDC, yNDC, zNDC, 1.0f);
			auto invertedPoint = modelView.getInverse() * projection.getInverse() * point4D;
			auto result = Vector3D(invertedPoint) / invertedPoint.w();

			auto expected = cursorPoint.unProject(modelView, projection, viewport);
			EXPECT_TRUE(result == expected);
		}
	}
}