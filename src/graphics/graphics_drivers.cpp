#include "graphics_drivers.hpp"

namespace cen {

auto get_render_driver_info(int index) noexcept
    -> std::optional<SDL_RendererInfo>
{
  SDL_RendererInfo info{};
  const auto result = SDL_GetRenderDriverInfo(index, &info);
  if (result == 0) {
    return info;
  } else {
    return std::nullopt;
  }
}

}  // namespace cen