#ifndef CENTURION_VULKAN_HEADER
#define CENTURION_VULKAN_HEADER

#ifndef CENTURION_NO_VULKAN

#include <SDL.h>
#include <SDL_vulkan.h>

#include <cassert>   // assert
#include <memory>    // unique_ptr
#include <optional>  // optional
#include <vector>    // vector

#include "../../core/czstring.hpp"
#include "../../core/integers.hpp"
#include "../../core/result.hpp"
#include "../window.hpp"

/// \addtogroup video
/// \{

/**
 * \namespace cen::vk
 *
 * \brief Contains Vulkan-related components.
 *
 * \since 6.0.0
 */
namespace cen::vk {

/**
 * \brief Returns the address of the `vkGetInstanceProcAddr` function.
 *
 * \return the address of the `vkGetInstanceProcAddr` function.
 *
 * \since 6.0.0
 */
[[nodiscard]] inline auto get_instance_proc_addr() noexcept -> void*
{
  return SDL_Vulkan_GetVkGetInstanceProcAddr();
}

/**
 * \brief Creates a rendering surface for a Vulkan window.
 *
 * \pre `window` must be a Vulkan window.
 *
 * \tparam T the ownership semantics of the window.
 *
 * \param window the Vulkan window.
 * \param instance the current Vulkan instance.
 * \param[out] outSurface the handle that will receive the handle of the created surface.
 *
 * \return `success` if the surface was succesfully created; `failure` otherwise.
 *
 * \since 6.0.0
 */
template <typename T>
auto create_surface(basic_window<T>& window,
                    VkInstance instance,
                    VkSurfaceKHR* outSurface) noexcept -> result
{
  assert(window.is_vulkan());
  return SDL_Vulkan_CreateSurface(window.get(), instance, outSurface) == SDL_TRUE;
}

/**
 * \brief Returns the extensions required to create a Vulkan surface.
 *
 * \return the required Vulkan extensions; `std::nullopt` if something goes wrong.
 *
 * \since 6.0.0
 */
inline auto required_extensions() -> std::optional<std::vector<czstring>>
{
  uint count{};
  if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &count, nullptr))
  {
    return std::nullopt;
  }

  std::vector<czstring> names(count);
  if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &count, names.data()))
  {
    return std::nullopt;
  }

  return names;
}

/**
 * \brief Returns the size of the drawable surface associated with the window.
 *
 * \pre `window` must be a Vulkan window.
 *
 * \tparam T the ownership semantics of the window.
 *
 * \param window the Vulkan window that will be queried.
 *
 * \return the size of the window drawable.
 *
 * \since 6.0.0
 */
template <typename T>
[[nodiscard]] auto drawable_size(const basic_window<T>& window) noexcept -> iarea
{
  assert(window.is_vulkan());

  int width{};
  int height{};

  SDL_Vulkan_GetDrawableSize(window.get(), &width, &height);

  return {width, height};
}

}  // namespace cen::vk

/// \} End of group video

#endif  // CENTURION_NO_VULKAN
#endif  // CENTURION_VULKAN_HEADER
