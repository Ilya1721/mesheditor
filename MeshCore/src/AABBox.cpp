module AABBox;

namespace MeshCore
{
	AABBox::AABBox() noexcept
	{
		init();
	}

	void AABBox::setFromMesh(const Mesh& mesh) noexcept
	{
		for (const auto& vertex : mesh.getVertices())
		{
			const auto& pos = vertex.pos;

			mMin.x = std::min(mMin.x, pos.x);
			mMax.x = std::max(mMax.x, pos.x);

			mMin.y = std::min(mMin.y, pos.y);
			mMax.y = std::max(mMax.y, pos.y);

			mMin.z = std::min(mMin.z, pos.z);
			mMax.z = std::max(mMax.z, pos.z);
		}
	}

	void AABBox::setFromObject(const Object3D& object) noexcept
	{
		setFromMesh(*object.getMesh());
		applyTransform(object.getLocalTransform());

		for (const auto& child : object.getChildren())
		{
			setFromObject(child);
		}
	}

	void AABBox::init() noexcept
	{
		constexpr auto floatMin = std::numeric_limits<float>::min();
		constexpr auto floatMax = std::numeric_limits<float>::max();
		mMax = glm::vec4(floatMin, floatMin, floatMin, 1.0f);
		mMin = glm::vec4(floatMax, floatMax, floatMax, 1.0f);
	}

	void AABBox::applyTransform(const glm::mat4& transform) noexcept
	{
		mMin = mMin * transform;
		mMax = mMax * transform;
	}

	glm::vec3 AABBox::getCenter() const noexcept
	{
		auto center = (mMin + mMax) / 2.0f;
		return center;
	}

	const glm::vec3& AABBox::getMin() const noexcept
	{
		return mMin;
	}

	const glm::vec3& AABBox::getMax() const noexcept
	{
		return mMax;
	}

	double AABBox::getHeight() const noexcept
	{
		return mMax.y - mMin.y;
	}
}
