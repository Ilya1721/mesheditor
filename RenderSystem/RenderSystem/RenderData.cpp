#include "RenderData.h"

namespace RenderSystem
{
  void RenderData::append(const RenderData& other)
  {
    mCompactData.insert(
      std::end(mCompactData), std::cbegin(other.mCompactData),
      std::cend(other.mCompactData)
    );
  }

  const std::vector<float>& RenderData::getCompactData() const
  {
    return mCompactData;
  }
}  // namespace RenderSystem
