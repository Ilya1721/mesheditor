#pragma once

#include <vector>

namespace RenderSystem
{
  class RenderData
  {
   public:
    virtual ~RenderData() = default;

    const std::vector<float>& getCompactData() const;
    void append(const RenderData& other);

   protected:
    std::vector<float> mCompactData;
  };
}  // namespace RenderSystem
