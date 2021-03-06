#include <gtest/gtest.h>

#include "system/cpu.hpp"

TEST(SIMDBlock, Usage)
{
  cen::simd_block block{100};
  ASSERT_TRUE(block);
  ASSERT_TRUE(block.data());
  ASSERT_TRUE(block.cast_data<int>());

  const auto& cblock = block;
  ASSERT_TRUE(cblock);
  ASSERT_TRUE(cblock.data());
  ASSERT_TRUE(cblock.cast_data<int>());

#if SDL_VERSION_ATLEAST(2, 0, 14)
  block.reallocate(50);
  ASSERT_TRUE(block);
  ASSERT_TRUE(block.data());
  ASSERT_TRUE(block.cast_data<int>());
#endif  // SDL_VERSION_ATLEAST(2, 0, 14)
}
