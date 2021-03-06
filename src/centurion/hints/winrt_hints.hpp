#ifndef CENTURION_WINRT_HINTS_HEADER
#define CENTURION_WINRT_HINTS_HEADER

#include <SDL.h>

#include "../core/czstring.hpp"
#include "../detail/hints_impl.hpp"

/// \addtogroup configuration
/// \{

namespace cen::hint::winrt {

struct privacy_policy_label final : detail::string_hint<privacy_policy_label>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_LABEL;
  }
};

struct privacy_policy_url final : detail::string_hint<privacy_policy_url>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_URL;
  }
};

struct handle_back_button final : detail::bool_hint<handle_back_button>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_HANDLE_BACK_BUTTON;
  }
};

}  // namespace cen::hint::winrt

/// \} End of group configuration

#endif  // CENTURION_WINRT_HINTS_HEADER