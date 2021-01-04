#include "rect.hpp"

#include <gtest/gtest.h>

#include "log.hpp"

TEST(Rect, DefaultConstructor)
{
  const cen::frect rect;
  EXPECT_EQ(rect.x(), 0);
  EXPECT_EQ(rect.y(), 0);
  EXPECT_EQ(rect.width(), 0);
  EXPECT_EQ(rect.height(), 0);
  EXPECT_FALSE(rect.has_area());
}

TEST(Rect, ValueConstructor)
{
  const cen::fpoint pos{123.5f, 81.4f};
  const cen::farea size{921.8f, 512.6f};
  const cen::frect rect{pos, size};

  EXPECT_EQ(rect.x(), pos.x());
  EXPECT_EQ(rect.y(), pos.y());
  EXPECT_EQ(rect.width(), size.width);
  EXPECT_EQ(rect.height(), size.height);

  EXPECT_NO_THROW(cen::frect({{0, 0}, {0, 0}}));
  EXPECT_NO_THROW(cen::frect({{0, 0}, {-1, -1}}));
}

TEST(Rect, SetX)
{
  cen::frect rect;

  const auto x = 123.4f;
  rect.set_x(x);

  EXPECT_EQ(rect.x(), x);
}

TEST(Rect, SetY)
{
  cen::frect rect;

  const auto y = 8527.1f;
  rect.set_y(y);

  EXPECT_EQ(rect.y(), y);
}

TEST(Rect, MoveTo)
{
  cen::frect rect;

  const cen::fpoint pos{742.3f, 377.2f};
  rect.move_to(pos);

  EXPECT_EQ(rect.position(), pos);
}

TEST(Rect, SetWidth)
{
  cen::frect rect;

  const auto width = 943.3f;
  rect.set_width(width);

  EXPECT_EQ(rect.width(), width);
}

TEST(Rect, SetHeight)
{
  cen::frect rect;

  const auto height = 62.35f;
  rect.set_height(height);

  EXPECT_EQ(rect.height(), height);
}

TEST(Rect, Resize)
{
  cen::frect rect;

  const cen::farea size{345.8f, 289.7f};
  rect.resize(size);

  EXPECT_EQ(rect.size(), size);
}

TEST(Rect, Contains)
{
  const cen::frect rect{{277.5f, 189.2f}, {79.2f, 58.2f}};

  {  // Top-left corner
    EXPECT_TRUE(rect.contains({rect.x(), rect.y()}));
    EXPECT_FALSE(rect.contains({rect.x() - 1, rect.y()}));
    EXPECT_FALSE(rect.contains({rect.x(), rect.y() - 1}));
  }

  {  // Top-right corner
    EXPECT_TRUE(rect.contains({rect.max_x(), rect.y()}));
    EXPECT_FALSE(rect.contains({rect.max_x() + 1, rect.y()}));
    EXPECT_FALSE(rect.contains({rect.max_x(), rect.y() - 1}));
  }

  {  // Bottom-left corner
    EXPECT_TRUE(rect.contains({rect.x(), rect.max_y()}));
    EXPECT_FALSE(rect.contains({rect.x() - 1, rect.max_y()}));
    EXPECT_FALSE(rect.contains({rect.x(), rect.max_y() + 1}));
  }

  {  // Bottom-right corner
    EXPECT_TRUE(rect.contains({rect.max_x(), rect.max_y()}));
    EXPECT_FALSE(rect.contains({rect.max_x() + 1, rect.max_y()}));
    EXPECT_FALSE(rect.contains({rect.max_y(), rect.max_y() + 1}));
  }
}

TEST(Rect, HasArea)
{
  {  // Default constructed rectangle
    const cen::frect rect;
    EXPECT_FALSE(rect.has_area());
  }

  {  // No width
    const cen::frect rect{{0, 0}, {0, 1}};
    EXPECT_FALSE(rect.has_area());
  }

  {  // No height
    const cen::frect rect{{0, 0}, {1, 0}};
    EXPECT_FALSE(rect.has_area());
  }

  {  // Negative dimensions
    const cen::frect rect{{0, 0}, {-1, -1}};
    EXPECT_FALSE(rect.has_area());
  }

  {  // Valid dimensions
    const cen::frect rect{{0, 0}, {1, 1}};
    EXPECT_TRUE(rect.has_area());
  }
}

TEST(Rect, X)
{
  const cen::frect rect;
  EXPECT_EQ(rect.x(), 0);
}

TEST(Rect, Y)
{
  const cen::frect rect;
  EXPECT_EQ(rect.y(), 0);
}

TEST(Rect, Width)
{
  const cen::frect rect;
  EXPECT_EQ(rect.width(), 0);
}

TEST(Rect, Height)
{
  const cen::frect rect;
  EXPECT_EQ(rect.height(), 0);
}

TEST(Rect, MaxX)
{
  const auto x = 289.2f;
  const auto width = 591.0f;

  const cen::frect rect{{x, 0}, {width, 0}};

  EXPECT_EQ(rect.max_x(), x + width);
}

TEST(Rect, MaxY)
{
  const auto y = 1029.3f;
  const auto height = 6961.9f;

  const cen::frect rect{{0, y}, {0, height}};

  EXPECT_EQ(rect.max_y(), y + height);
}

TEST(Rect, CenterX)
{
  const auto x = 125.3f;
  const auto width = 3912.8f;

  const cen::frect rect{{x, 0}, {width, 0}};

  EXPECT_EQ(rect.center_x(), x + (width / 2.0f));
}

TEST(Rect, CenterY)
{
  const auto y = 7128.2f;
  const auto height = 1240.2f;

  const cen::frect rect{{0, y}, {0, height}};

  EXPECT_EQ(rect.center_y(), y + (height / 2.0f));
}

TEST(Rect, Area)
{
  const auto width = 184.3f;
  const auto height = 728.9f;

  const cen::frect rect{{}, {width, height}};

  EXPECT_EQ(rect.area(), width * height);
}

TEST(Rect, Center)
{
  const auto x = 77;
  const auto y = 81;
  const auto w = 128;
  const auto h = 256;

  const cen::irect rect{{x, y}, {w, h}};
  const auto center = rect.center();

  EXPECT_EQ(center.x(), x + (w / 2));
  EXPECT_EQ(center.y(), y + (h / 2));
}

TEST(Rect, Intersects)
{
  const cen::frect rect{{100.0f, 100.0f}, {100.0f, 100.0f}};
  EXPECT_TRUE(cen::intersects(rect, rect));

  {  // Empty rectangle
    const cen::frect empty;
    EXPECT_FALSE(cen::intersects(empty, empty));
    EXPECT_FALSE(cen::intersects(rect, empty));
    EXPECT_FALSE(cen::intersects(empty, rect));
  }

  {  // Obviously no intersection
    const cen::frect left{{rect.x() - rect.width(), rect.y()}, {10, 10}};
    const cen::frect top{{rect.x(), rect.y() - rect.height()}, {10, 10}};
    const cen::frect right{{rect.x() + rect.width(), rect.y()},
                           {rect.width(), rect.height()}};
    const cen::frect bottom{{rect.x(), rect.y() + rect.height()}, {10, 10}};

    EXPECT_FALSE(cen::intersects(left, rect));
    EXPECT_FALSE(cen::intersects(rect, left));

    EXPECT_FALSE(cen::intersects(top, rect));
    EXPECT_FALSE(cen::intersects(rect, top));

    EXPECT_FALSE(cen::intersects(right, rect));
    EXPECT_FALSE(cen::intersects(rect, right));

    EXPECT_FALSE(cen::intersects(bottom, rect));
    EXPECT_FALSE(cen::intersects(rect, bottom));
  }

  {  // Edge cases
    const cen::frect left{{90, 100}, {10, 10}};
    EXPECT_FALSE(cen::intersects(left, rect));
    EXPECT_FALSE(cen::intersects(rect, left));

    const cen::frect top{{100, 90}, {10, 10}};
    EXPECT_FALSE(cen::intersects(top, rect));
    EXPECT_FALSE(cen::intersects(rect, top));

    const cen::frect right{{200, 100}, {10, 10}};
    EXPECT_FALSE(cen::intersects(right, rect));
    EXPECT_FALSE(cen::intersects(rect, right));

    const cen::frect bottom{{100, 200}, {10, 10}};
    EXPECT_FALSE(cen::intersects(bottom, rect));
    EXPECT_FALSE(cen::intersects(rect, bottom));
  }

  {  // Obvious intersections
    const cen::frect left{{90, 150}, {50, 1}};
    EXPECT_TRUE(cen::intersects(left, rect));
    EXPECT_TRUE(cen::intersects(rect, left));

    const cen::frect top{{150, 90}, {1, 50}};
    EXPECT_TRUE(cen::intersects(top, rect));
    EXPECT_TRUE(cen::intersects(rect, top));

    const cen::frect bottom{{150, 150}, {10, 50}};
    EXPECT_TRUE(cen::intersects(bottom, rect));
    EXPECT_TRUE(cen::intersects(rect, bottom));

    const cen::frect right{{150, 150}, {50, 10}};
    EXPECT_TRUE(cen::intersects(right, rect));
    EXPECT_TRUE(cen::intersects(rect, right));
  }
}

TEST(Rect, Collides)
{
  const cen::frect rect{{100.0f, 100.0f}, {100.0f, 100.0f}};
  EXPECT_TRUE(cen::collides(rect, rect));

  {  // Obviously no collisions
    const cen::frect left{{rect.x() - rect.width() - 1, rect.y()}, {10, 10}};
    const cen::frect top{{rect.x(), rect.y() - rect.height() - 1}, {10, 10}};
    const cen::frect right{{rect.x() + rect.width() + 1, rect.y()},
                           {rect.width(), rect.height()}};
    const cen::frect bottom{{rect.x(), rect.y() + rect.height() + 1}, {10, 10}};

    EXPECT_FALSE(cen::collides(left, rect));
    EXPECT_FALSE(cen::collides(rect, left));

    EXPECT_FALSE(cen::collides(top, rect));
    EXPECT_FALSE(cen::collides(rect, top));

    EXPECT_FALSE(cen::collides(right, rect));
    EXPECT_FALSE(cen::collides(rect, right));

    EXPECT_FALSE(cen::collides(bottom, rect));
    EXPECT_FALSE(cen::collides(rect, bottom));
  }

  {  // Edge cases
    const cen::frect left{{89, 100}, {10, 10}};
    EXPECT_FALSE(cen::collides(left, rect));
    EXPECT_FALSE(cen::collides(rect, left));

    const cen::frect top{{100, 89}, {10, 10}};
    EXPECT_FALSE(cen::collides(top, rect));
    EXPECT_FALSE(cen::collides(rect, top));

    const cen::frect right{{201, 100}, {10, 10}};
    EXPECT_FALSE(cen::collides(right, rect));
    EXPECT_FALSE(cen::collides(rect, right));

    const cen::frect bottom{{100, 201}, {10, 10}};
    EXPECT_FALSE(cen::collides(bottom, rect));
    EXPECT_FALSE(cen::collides(rect, bottom));
  }

  {  // Obvious collisions
    const cen::frect left{{90, 150}, {50, 1}};
    EXPECT_TRUE(cen::collides(left, rect));
    EXPECT_TRUE(cen::collides(rect, left));

    const cen::frect top{{150, 90}, {1, 50}};
    EXPECT_TRUE(cen::collides(top, rect));
    EXPECT_TRUE(cen::collides(rect, top));

    const cen::frect bottom{{150, 150}, {10, 50}};
    EXPECT_TRUE(cen::collides(bottom, rect));
    EXPECT_TRUE(cen::collides(rect, bottom));

    const cen::frect right{{150, 150}, {50, 10}};
    EXPECT_TRUE(cen::collides(right, rect));
    EXPECT_TRUE(cen::collides(rect, right));
  }
}

TEST(Rect, ToString)
{
  const cen::frect rect{{14.3f, 34.2f}, {182.8f, 120.9f}};
  cen::log::put(cen::to_string(rect));
}

TEST(Rect, GetUnion)
{
  const cen::frect fst{{10.0f, 10.0f}, {50.0f, 50.0f}};
  const cen::frect snd{{40.0f, 40.0f}, {50.0f, 50.0f}};

  {  // With empty rectangle
    const cen::frect empty;

    EXPECT_EQ(cen::get_union(empty, empty), empty);
    EXPECT_EQ(cen::get_union(empty, fst), fst);
    EXPECT_EQ(cen::get_union(fst, empty), fst);
  }

  const auto fstSnd = cen::get_union(fst, snd);
  const auto sndFst = cen::get_union(snd, fst);

  EXPECT_TRUE(fstSnd.has_area());

  EXPECT_EQ(fstSnd.x(), 10.0f);
  EXPECT_EQ(fstSnd.y(), 10.0f);
  EXPECT_EQ(fstSnd.width(), 80.0f);
  EXPECT_EQ(fstSnd.height(), 80.0f);

  EXPECT_EQ(fstSnd, sndFst);
  EXPECT_EQ(sndFst, fstSnd);
}

TEST(Rect, IRectToFRect)
{
  const cen::irect source{{78, 12}, {283, 313}};
  const auto result = cen::cast<cen::frect>(source);

  EXPECT_EQ(result.x(), static_cast<float>(source.x()));
  EXPECT_EQ(result.y(), static_cast<float>(source.y()));
  EXPECT_EQ(result.width(), static_cast<float>(source.width()));
  EXPECT_EQ(result.height(), static_cast<float>(source.height()));
}

TEST(Rect, FRectToIRect)
{
  const cen::frect source{{831.3f, 899.1f}, {67.2f, 91.7f}};
  const auto result = cen::cast<cen::irect>(source);

  EXPECT_EQ(result.x(), static_cast<int>(source.x()));
  EXPECT_EQ(result.y(), static_cast<int>(source.y()));
  EXPECT_EQ(result.width(), static_cast<int>(source.width()));
  EXPECT_EQ(result.height(), static_cast<int>(source.height()));
}

TEST(Rect, FRectToPointer)
{
  const cen::frect rect{{78.3f, 623.2f}, {99.13f, 64.3f}};
  const auto* ptr = static_cast<const SDL_FRect*>(rect);

  EXPECT_EQ(rect.x(), ptr->x);
  EXPECT_EQ(rect.y(), ptr->y);
  EXPECT_EQ(rect.width(), ptr->w);
  EXPECT_EQ(rect.height(), ptr->h);
}

TEST(Rect, RectToPointer)
{
  const cen::irect rect{{123, 321}, {782, 991}};
  const auto* ptr = static_cast<const SDL_Rect*>(rect);

  EXPECT_EQ(rect.x(), ptr->x);
  EXPECT_EQ(rect.y(), ptr->y);
  EXPECT_EQ(rect.width(), ptr->w);
  EXPECT_EQ(rect.height(), ptr->h);
}

TEST(Rect, EqualityOperatorReflexivity)
{
  const cen::frect rect{{93.3f, 67.2f}, {54.2f, 777.8f}};
  EXPECT_EQ(rect, rect);
}

TEST(Rect, EqualityOperatorComparison)
{
  const cen::frect fst{{78.2f, 21.2f}, {9.2f, 162.3f}};
  const cen::frect snd{fst};
  EXPECT_EQ(fst, snd);
  EXPECT_EQ(snd, fst);
}

TEST(Rect, EqualityOperatorComparisonDifferent)
{
  const cen::frect fst{{8.2f, 123.3f}, {63.1f, 672.3f}};
  const cen::frect snd{{89.13f, 781.3f}, {781.2f, 331.3f}};
  EXPECT_FALSE(fst == snd);
  EXPECT_FALSE(snd == fst);
}

TEST(Rect, InequalityOperatorSelf)
{
  const cen::frect rect{{21.7f, 32.2f}, {442.2f, 383.8f}};
  EXPECT_FALSE(rect != rect);
}

TEST(Rect, InequalityOperatorComparisonEqual)
{
  const cen::frect fst{{712.3f, 34.3f}, {65.8f, 348.2f}};
  const cen::frect snd{fst};
  EXPECT_FALSE(fst != snd);
  EXPECT_FALSE(snd != fst);
}

TEST(Rect, InequalityOperatorComparisonDifferent)
{
  const cen::frect fst{{-45.37f, 12.3f}, {89.13f, 371.3f}};
  const cen::frect snd{{738.3f, 8.24f}, {67.3f, 89.23f}};
  EXPECT_NE(fst, snd);
  EXPECT_NE(snd, fst);
}