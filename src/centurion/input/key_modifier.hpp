#ifndef CENTURION_KEY_MODIFIER_HEADER
#define CENTURION_KEY_MODIFIER_HEADER

#include <SDL.h>

#include "../core/integers.hpp"

namespace cen {

/// \addtogroup input
/// \{

/**
 * \enum key_modifier
 *
 * \brief Provides values that represent different key modifiers.
 *
 * \see `SDL_Keymod`
 *
 * \since 3.1.0
 */
enum class key_modifier : u16
{
  none = KMOD_NONE,
  left_shift = KMOD_LSHIFT,
  right_shift = KMOD_RSHIFT,
  left_ctrl = KMOD_LCTRL,
  right_ctrl = KMOD_RCTRL,
  left_alt = KMOD_LALT,
  right_alt = KMOD_RALT,
  left_gui = KMOD_LGUI,
  right_gui = KMOD_RGUI,
  num = KMOD_NUM,
  caps = KMOD_CAPS,
  mode = KMOD_MODE
};

/// \} End of group input

}  // namespace cen

#endif  // CENTURION_KEY_MODIFIER_HEADER