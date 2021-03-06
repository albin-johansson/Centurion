#include "thread/semaphore.hpp"

#include <gtest/gtest.h>

TEST(Semaphore, Acquire)
{
  cen::semaphore semaphore{1};

  ASSERT_TRUE(semaphore.acquire());
  ASSERT_EQ(semaphore.tokens(), 0);

  ASSERT_TRUE(semaphore.release());
  ASSERT_EQ(semaphore.tokens(), 1);
}

TEST(Semaphore, AcquireMilliseconds)
{
  using ms = cen::milliseconds<cen::u32>;

  cen::semaphore semaphore{0};

  ASSERT_EQ(semaphore.acquire(ms{1}), cen::lock_status::timed_out);
  ASSERT_TRUE(semaphore.release());

  ASSERT_EQ(semaphore.acquire(ms{1}), cen::lock_status::success);
}

TEST(Semaphore, TryAcquire)
{
  cen::semaphore semaphore{0};

  ASSERT_EQ(semaphore.try_acquire(), cen::lock_status::timed_out);
  ASSERT_TRUE(semaphore.release());

  ASSERT_EQ(semaphore.try_acquire(), cen::lock_status::success);
}

TEST(Semaphore, Release)
{
  cen::semaphore semaphore{0};

  ASSERT_EQ(semaphore.tokens(), 0);
  ASSERT_TRUE(semaphore.release());
  ASSERT_EQ(semaphore.tokens(), 1);
}

TEST(Semaphore, Tokens)
{
  constexpr auto tokens = 32;

  cen::semaphore semaphore{tokens};
  ASSERT_EQ(semaphore.tokens(), tokens);
}
