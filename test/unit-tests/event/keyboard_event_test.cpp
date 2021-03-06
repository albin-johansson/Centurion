#include <gtest/gtest.h>

#include "events/event.hpp"

TEST(KeyboardEvent, Defaults)
{
  cen::keyboard_event event;
  ASSERT_GT(event.time(), 0u);
  ASSERT_EQ(cen::event_type::key_down, event.type());
}

TEST(KeyboardEvent, SetScanCode)
{
  cen::keyboard_event event;

  event.set_scan_code(cen::scancodes::b);
  ASSERT_EQ(cen::scancodes::b, event.get_scan_code());
}

TEST(KeyboardEvent, SetKeyCode)
{
  cen::keyboard_event event;

  event.set_key_code(cen::keycodes::n);
  ASSERT_EQ(cen::keycodes::n, event.get_key_code());
}

TEST(KeyboardEvent, SetModifier)
{
  cen::keyboard_event event;

  constexpr auto shift = cen::key_modifier::left_shift;
  constexpr auto caps = cen::key_modifier::caps;

  event.set_modifier(shift, true);
  ASSERT_TRUE(event.modifier_active(shift));

  event.set_modifier(caps, true);
  ASSERT_TRUE(event.modifier_active(shift));
  ASSERT_TRUE(event.modifier_active(caps));

  event.set_modifier(shift, false);
  ASSERT_FALSE(event.modifier_active(shift));
  ASSERT_TRUE(event.modifier_active(caps));
}

TEST(KeyboardEvent, SetRepeated)
{
  cen::keyboard_event event;

  event.set_repeated(true);
  ASSERT_TRUE(event.repeated());

  event.set_repeated(false);
  ASSERT_FALSE(event.repeated());
}

TEST(KeyboardEvent, SetWindowId)
{
  cen::keyboard_event event;

  constexpr auto id = 79;
  event.set_window_id(id);

  ASSERT_EQ(id, event.window_id());
}

TEST(KeyboardEvent, IsActive)
{
  SDL_KeyboardEvent sdl{};
  sdl.keysym.scancode = SDL_SCANCODE_Q;
  sdl.keysym.sym = SDLK_d;

  const cen::keyboard_event event{sdl};

  ASSERT_TRUE(event.is_active(cen::keycodes::d));
  ASSERT_TRUE(event.is_active(cen::scancodes::q));

  ASSERT_FALSE(event.is_active(cen::keycodes::x));
  ASSERT_FALSE(event.is_active(cen::scancodes::o));
}

TEST(KeyboardEvent, ModifierActive)
{
  SDL_KeyboardEvent sdl{};

  SDL_Keysym keysym{};
  keysym.mod = KMOD_LALT | KMOD_CAPS;

  sdl.keysym = keysym;

  const cen::keyboard_event event{sdl};

  // Check that multiple key modifiers can be active at the same time
  ASSERT_TRUE(event.modifier_active(cen::key_modifier::left_alt));
  ASSERT_TRUE(event.modifier_active(cen::key_modifier::caps));
}

TEST(KeyboardEvent, ShiftActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.shift_active());
  }

  {  // One modifier (Both LSHIFT and RSHIFT)
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_SHIFT;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.shift_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_RSHIFT | KMOD_CAPS | KMOD_LGUI;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.shift_active());
  }
}

TEST(KeyboardEvent, CtrlActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.ctrl_active());
  }

  {  // One modifier (Both LCTRL and RCTRL)
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_CTRL;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.ctrl_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_LCTRL | KMOD_ALT | KMOD_LGUI;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.ctrl_active());
  }
}

TEST(KeyboardEvent, AltActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.alt_active());
  }

  {  // One modifier (Both LALT and RALT)
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_ALT;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.alt_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_RALT | KMOD_RSHIFT | KMOD_CAPS;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.alt_active());
  }
}

TEST(KeyboardEvent, GuiActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.gui_active());
  }

  {  // One modifier (Both LGUI and RGUI)
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_GUI;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.gui_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_LGUI | KMOD_RSHIFT | KMOD_CAPS;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.gui_active());
  }
}

TEST(KeyboardEvent, CapsActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.caps_active());
  }

  {  // One modifier
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_CAPS;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.caps_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_CAPS | KMOD_RSHIFT | KMOD_LCTRL;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.caps_active());
  }
}

TEST(KeyboardEvent, NumActive)
{
  {  // No modifiers
    const cen::keyboard_event event;
    ASSERT_FALSE(event.num_active());
  }

  {  // One modifier
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_NUM;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.num_active());
  }

  {  // With other modifiers
    SDL_KeyboardEvent sdl{};

    SDL_Keysym keysym{};
    keysym.mod = KMOD_NUM | KMOD_RSHIFT | KMOD_LCTRL;

    sdl.keysym = keysym;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.num_active());
  }
}

TEST(KeyboardEvent, Repeated)
{
  const auto createEvent = [](const int repeats) noexcept {
    SDL_KeyboardEvent sdl{};
    sdl.repeat = static_cast<cen::u8>(repeats);
    return cen::keyboard_event{sdl};
  };

  const auto noRepeat = createEvent(0);
  const auto oneRepeat = createEvent(1);
  const auto twoRepeats = createEvent(2);

  ASSERT_FALSE(noRepeat.repeated());
  ASSERT_TRUE(oneRepeat.repeated());
  ASSERT_TRUE(twoRepeats.repeated());
}

TEST(KeyboardEvent, State)
{
  {  // Default button state
    const cen::keyboard_event event;
    ASSERT_EQ(cen::button_state::released, event.state());
  }

  {  // Check valid state
    SDL_KeyboardEvent sdl{};

    sdl.keysym.sym = SDLK_ESCAPE;
    sdl.state = SDL_PRESSED;

    const cen::keyboard_event event{sdl};
    ASSERT_EQ(cen::button_state::pressed, event.state());
  }
}

TEST(KeyboardEvent, Released)
{
  {  // Released
    SDL_KeyboardEvent sdl{};
    sdl.state = SDL_RELEASED;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.released());
    ASSERT_EQ(cen::button_state::released, event.state());
  }

  {  // Not released
    SDL_KeyboardEvent sdl{};
    sdl.state = SDL_PRESSED;

    const cen::keyboard_event event{sdl};
    ASSERT_FALSE(event.released());
  }
}

TEST(KeyboardEvent, Pressed)
{
  {  // Pressed
    SDL_KeyboardEvent sdl{};
    sdl.state = SDL_PRESSED;

    const cen::keyboard_event event{sdl};
    ASSERT_TRUE(event.pressed());
    ASSERT_EQ(cen::button_state::pressed, event.state());
  }

  {  // Not pressed
    SDL_KeyboardEvent sdl{};
    sdl.state = SDL_RELEASED;

    const cen::keyboard_event event{sdl};
    ASSERT_FALSE(event.pressed());
  }
}

TEST(KeyboardEvent, GetScanCode)
{
  cen::keyboard_event event;

  constexpr auto code = cen::scancodes::q;
  event.set_scan_code(code);

  ASSERT_EQ(code, event.get_scan_code());
}

TEST(KeyboardEvent, GetKeyCode)
{
  cen::keyboard_event event;

  constexpr auto code = cen::keycodes::x;
  event.set_key_code(code);

  ASSERT_EQ(code, event.get_key_code());
}

TEST(KeyboardEvent, WindowId)
{
  SDL_KeyboardEvent sdl{};
  sdl.windowID = 72;

  const cen::keyboard_event event{sdl};
  ASSERT_EQ(sdl.windowID, event.window_id());
}

TEST(KeyboardEvent, AsSDLEvent)
{
  const cen::keyboard_event event;
  const auto sdl = cen::as_sdl_event(event);

  ASSERT_EQ(sdl.key.type, static_cast<cen::u32>(event.type()));
  ASSERT_EQ(sdl.key.timestamp, event.time());
}
