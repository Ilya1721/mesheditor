#include <gtest/gtest.h>

#include "TestUtils.h"

#include <glm/glm.hpp>

#include "MeshCore/Object3D.h"

namespace Object3D
{
	TEST(Constructors, DefaultConstructor)
	{
		MeshCore::Object3D object;
		EXPECT_TRUE(object.getParent() == nullptr);
		EXPECT_TRUE(object.getChildren().empty());
		EXPECT_TRUE(object.getMesh().getVertices().empty());
		EXPECT_TRUE(object.getRenderData().getCompactData().empty());
		EXPECT_TRUE(object.getTransform() == glm::mat4(1.0f));
	}

	TEST(Constructors, ParentMeshConstructor)
	{
		MeshCore::Object3D parent;
		MeshCore::Object3D object(&parent, getDefaultMesh());
		EXPECT_TRUE(object.getParent() == &parent);
		const auto& parentChildren = parent.getChildren();
		EXPECT_TRUE(parentChildren.find(&object) != parentChildren.end());
		EXPECT_TRUE(object.getMesh().getVertices()[0] == defaultVertex);
		EXPECT_TRUE(object.getTransform() == glm::mat4(1.0f));
	}

	TEST(Constructors, MoveConstructor)
	{
		auto orig = getDefaultObject3D();
		MeshCore::Object3D copy(getDefaultObject3D());
		EXPECT_TRUE(orig == copy);
	}

	TEST(Methods, AppendChild)
	{
		MeshCore::Object3D parent;
		MeshCore::Object3D child;
		parent.appendChild(&child);
		EXPECT_TRUE(child.getParent() == &parent);
		const auto& parentChildren = parent.getChildren();
		EXPECT_TRUE(parentChildren.find(&child) != parentChildren.end());
		child.appendChild(nullptr);
		EXPECT_TRUE(child.getChildren().empty());
	}

	TEST(Methods, RemoveChild)
	{
		MeshCore::Object3D parent;
		MeshCore::Object3D child;
		parent.appendChild(&child);
		parent.removeChild(&child);
		EXPECT_TRUE(child.getParent() == nullptr);
		EXPECT_TRUE(parent.getChildren().empty());
	}

	TEST(Methods, GetRenderData)
	{
		MeshCore::Object3D parent(nullptr, getDefaultMesh());
		auto combinedRenderData = parent.getRenderData();
		MeshCore::Object3D child(&parent, getDefaultMesh());
		combinedRenderData.append(child.getRenderData());
		EXPECT_TRUE(combinedRenderData == parent.getRenderData());
	}

	TEST(Methods, GetTransform)
	{
		MeshCore::Object3D object;
		EXPECT_TRUE(object.getTransform() == glm::mat4(1.0f));
	}
}