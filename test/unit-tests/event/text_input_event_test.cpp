#include <gtest/gtest.h>

#include "events/event.hpp"

TEST(TextInputEvent, Defaults)
{
  cen::text_input_event event;
  ASSERT_GT(event.time(), 0u);
  ASSERT_EQ(cen::event_type::text_input, event.type());
}

TEST(TextInputEvent, SetWindowId)
{
  cen::text_input_event event;

  constexpr auto id = 23;
  event.set_window_id(id);

  ASSERT_EQ(id, event.window_id());
}

TEST(TextInputEvent, WindowId)
{
  SDL_TextInputEvent sdl{};
  sdl.type = SDL_TEXTINPUT;
  sdl.windowID = 8;

  const cen::text_input_event event{sdl};
  ASSERT_EQ(sdl.windowID, event.window_id());
}

TEST(TextInputEvent, TextUtf8)
{
  const SDL_TextInputEvent sdl{SDL_TEXTINPUT, 1, 1, "hello"};
  const cen::text_input_event event{sdl};
  ASSERT_EQ("hello", event.text_utf8());
}

TEST(TextInputEvent, AsSDLEvent)
{
  const cen::text_input_event event;
  const auto sdl = cen::as_sdl_event(event);

  ASSERT_EQ(sdl.text.type, static_cast<cen::u32>(event.type()));
  ASSERT_EQ(sdl.text.timestamp, event.time());
}
