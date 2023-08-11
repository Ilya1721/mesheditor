#include <gtest/gtest.h>

#include "MeshCore/RenderData.h"

namespace RenderData
{
	TEST(Methods, AppendCoord) 
	{
		MeshCore::RenderData renderData;
		renderData.append(1.0f, 1.0f);
		EXPECT_FLOAT_EQ(renderData.positions[0], 1.0f);
		EXPECT_FLOAT_EQ(renderData.normals[0], 1.0f);
	}

	TEST(Methods, AppendRenderData)
	{
		MeshCore::RenderData renderData;
		renderData.append(1.0f, 1.0f);
		MeshCore::RenderData secondRenderData;
		secondRenderData.append(renderData);
		EXPECT_FLOAT_EQ(secondRenderData.positions[0], 1.0f);
		EXPECT_FLOAT_EQ(secondRenderData.normals[0], 1.0f);
	}

	TEST(Methods, GetCompactData)
	{
		MeshCore::RenderData renderData;
		renderData.append(1.0f, 2.0f);
		renderData.append(3.0f, 4.0f);
		auto compactData = renderData.getCompactData();
		EXPECT_FLOAT_EQ(compactData[0], 1.0f);
		EXPECT_FLOAT_EQ(compactData[1], 3.0f);
		EXPECT_FLOAT_EQ(compactData[2], 2.0f);
		EXPECT_FLOAT_EQ(compactData[3], 4.0f);
	}
}