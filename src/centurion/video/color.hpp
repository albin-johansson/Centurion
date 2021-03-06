#ifndef CENTURION_COLOR_HEADER
#define CENTURION_COLOR_HEADER

#include <SDL.h>

#include <cassert>  // assert
#include <cmath>    // round, fabs, fmod
#include <ostream>  // ostream
#include <string>   // string

#include "../core/integers.hpp"
#include "../detail/to_string.hpp"

namespace cen {

/// \addtogroup video
/// \{

/**
 * \class color
 *
 * \brief An 8-bit accuracy RGBA color.
 *
 * \details This class is designed to interact with the SDL colors, i.e. `SDL_Color` and
 * `SDL_MessageBoxColor`. For convenience, there are approximately 140 color constants
 * provided in the `cen::colors` namespace,
 *
 * \since 3.0.0
 */
class color final
{
 public:
  /// \name Construction
  /// \{

  /**
   * \brief Creates a color. The created color will be equal to #000000FF.
   *
   * \since 3.0.0
   */
  constexpr color() noexcept = default;

  /**
   * \brief Creates a color.
   *
   * \param red the red component value, in the range [0, 255].
   * \param green the green component value, in the range [0, 255].
   * \param blue the blue component value, in the range [0, 255].
   * \param alpha the alpha component value, in the rage [0, 255]. Defaults to 255.
   *
   * \since 3.0.0
   */
  constexpr color(const u8 red,
                  const u8 green,
                  const u8 blue,
                  const u8 alpha = max()) noexcept
      : m_color{red, green, blue, alpha}
  {}

  /**
   * \brief Creates a color that is a copy of the supplied `SDL_Color`.
   *
   * \param color the `SDL_Color` that will be copied.
   *
   * \since 3.0.0
   */
  constexpr explicit color(const SDL_Color& color) noexcept : m_color{color}
  {}

  /**
   * \brief Creates a color that is a copy of the supplied SDL_MessageBoxColor.
   *
   * \details Message box colors don't have an alpha component so the created color will
   * feature an alpha value of 255.
   *
   * \param color the message box color that will be copied.
   *
   * \since 3.0.0
   */
  constexpr explicit color(const SDL_MessageBoxColor& color) noexcept
      : m_color{color.r, color.g, color.b, max()}
  {}

  /**
   * \brief Creates a color from HSV-encoded values.
   *
   * \pre `hue` must be in the range [0, 360].
   * \pre `saturation` must be in the range [0, 100].
   * \pre `value` must be in the range [0, 100].
   *
   * \param hue the hue of the color, in the range [0, 360].
   * \param saturation the saturation of the color, in the range [0, 100].
   * \param value the value of the color, in the range [0, 100].
   *
   * \return an RGBA color converted from the HSV values.
   *
   * \since 5.3.0
   */
  [[nodiscard]] static auto from_hsv(const double hue,
                                     const double saturation,
                                     const double value) -> color
  {
    assert(hue >= 0);
    assert(hue <= 360);
    assert(saturation >= 0);
    assert(saturation <= 100);
    assert(value >= 0);
    assert(value <= 100);

    const auto v = (value / 100.0);
    const auto chroma = v * (saturation / 100.0);
    const auto hp = hue / 60.0;

    const auto x = chroma * (1.0 - std::fabs(std::fmod(hp, 2.0) - 1.0));

    double red{};
    double green{};
    double blue{};

    if (0 <= hp && hp <= 1)
    {
      red = chroma;
      green = x;
      blue = 0;
    }
    else if (1 < hp && hp <= 2)
    {
      red = x;
      green = chroma;
      blue = 0.0;
    }
    else if (2 < hp && hp <= 3)
    {
      red = 0;
      green = chroma;
      blue = x;
    }
    else if (3 < hp && hp <= 4)
    {
      red = 0;
      green = x;
      blue = chroma;
    }
    else if (4 < hp && hp <= 5)
    {
      red = x;
      green = 0;
      blue = chroma;
    }
    else if (5 < hp && hp <= 6)
    {
      red = chroma;
      green = 0;
      blue = x;
    }

    const auto m = v - chroma;

    const auto r = static_cast<u8>(std::round((red + m) * 255.0));
    const auto g = static_cast<u8>(std::round((green + m) * 255.0));
    const auto b = static_cast<u8>(std::round((blue + m) * 255.0));

    return color{r, g, b};
  }

  /**
   * \brief Creates a color from HSL-encoded values.
   *
   * \pre `hue` must be in the range [0, 360].
   * \pre `saturation` must be in the range [0, 100].
   * \pre `lightness` must be in the range [0, 100].
   *
   * \param hue the hue of the color, in the range [0, 360].
   * \param saturation the saturation of the color, in the range [0, 100].
   * \param lightness the lightness of the color, in the range [0, 100].
   *
   * \return an RGBA color converted from the HSL values.
   *
   * \since 5.3.0
   */
  [[nodiscard]] static auto from_hsl(const double hue,
                                     const double saturation,
                                     const double lightness) -> color
  {
    assert(hue >= 0);
    assert(hue <= 360);
    assert(saturation >= 0);
    assert(saturation <= 100);
    assert(lightness >= 0);
    assert(lightness <= 100);

    const auto s = saturation / 100.0;
    const auto l = lightness / 100.0;

    const auto chroma = (1.0 - std::fabs(2.0 * l - 1)) * s;
    const auto hp = hue / 60.0;

    const auto x = chroma * (1 - std::fabs(std::fmod(hp, 2.0) - 1.0));

    double red{};
    double green{};
    double blue{};

    if (0 <= hp && hp < 1)
    {
      red = chroma;
      green = x;
      blue = 0;
    }
    else if (1 <= hp && hp < 2)
    {
      red = x;
      green = chroma;
      blue = 0;
    }
    else if (2 <= hp && hp < 3)
    {
      red = 0;
      green = chroma;
      blue = x;
    }
    else if (3 <= hp && hp < 4)
    {
      red = 0;
      green = x;
      blue = chroma;
    }
    else if (4 <= hp && hp < 5)
    {
      red = x;
      green = 0;
      blue = chroma;
    }
    else if (5 <= hp && hp < 6)
    {
      red = chroma;
      green = 0;
      blue = x;
    }

    const auto m = l - (chroma / 2.0);

    const auto r = static_cast<u8>(std::round((red + m) * 255.0));
    const auto g = static_cast<u8>(std::round((green + m) * 255.0));
    const auto b = static_cast<u8>(std::round((blue + m) * 255.0));

    return color{r, g, b};
  }

  /// \} End of construction

  /// \name Setters
  /// \{

  /**
   * \brief Sets the value of the red component.
   *
   * \param red the new value of the red component.
   *
   * \since 3.0.0
   */
  constexpr void set_red(const u8 red) noexcept
  {
    m_color.r = red;
  }

  /**
   * \brief Sets the value of the green component.
   *
   * \param green the new value of the green component.
   *
   * \since 3.0.0
   */
  constexpr void set_green(const u8 green) noexcept
  {
    m_color.g = green;
  }

  /**
   * \brief Sets the value of the blue component.
   *
   * \param blue the new value of the blue component.
   *
   * \since 3.0.0
   */
  constexpr void set_blue(const u8 blue) noexcept
  {
    m_color.b = blue;
  }

  /**
   * \brief Sets the value of the alpha component.
   *
   * \param alpha the new value of the alpha component.
   *
   * \since 3.0.0
   */
  constexpr void set_alpha(const u8 alpha) noexcept
  {
    m_color.a = alpha;
  }

  /// \} End of setters

  /// \name Getters
  /// \{

  /**
   * \brief Returns the value of the red component.
   *
   * \return the value of the red component, in the range [0, 255].
   *
   * \since 3.0.0
   */
  [[nodiscard]] constexpr auto red() const noexcept -> u8
  {
    return m_color.r;
  }

  /**
   * \brief Returns the value of the green component.
   *
   * \return the value of the green component, in the range [0, 255].
   *
   * \since 3.0.0
   */
  [[nodiscard]] constexpr auto green() const noexcept -> u8
  {
    return m_color.g;
  }

  /**
   * \brief Returns the value of the blue component.
   *
   * \return the value of the blue component, in the range [0, 255].
   *
   * \since 3.0.0
   */
  [[nodiscard]] constexpr auto blue() const noexcept -> u8
  {
    return m_color.b;
  }

  /**
   * \brief Returns the value of the alpha component.
   *
   * \return the value of the alpha component, in the range [0, 255].
   *
   * \since 3.0.0
   */
  [[nodiscard]] constexpr auto alpha() const noexcept -> u8
  {
    return m_color.a;
  }

  /**
   * \brief Returns a pointer to the internal SDL color.
   *
   * \warning Do not cache the returned pointer!
   *
   * \return a pointer to the internal color instance.
   *
   * \since 6.0.0
   */
  [[nodiscard]] auto data() noexcept -> SDL_Color*
  {
    return &m_color;
  }

  /// \copydoc data()
  [[nodiscard]] auto data() const noexcept -> const SDL_Color*
  {
    return &m_color;
  }

  /**
   * \brief Returns the internal color instance.
   *
   * \return a reference to the internal color.
   *
   * \since 5.0.0
   */
  [[nodiscard]] auto get() const noexcept -> const SDL_Color&
  {
    return m_color;
  }

  /// \} End of getters

  /// \name Conversions
  /// \{

  /**
   * \brief Converts the the color into an `SDL_Color`.
   *
   * \return an `SDL_Color` that is equivalent to this color.
   *
   * \since 3.0.0
   */
  [[nodiscard]] explicit constexpr operator SDL_Color() const noexcept
  {
    return {red(), green(), blue(), alpha()};
  }

  /**
   * \brief Converts the the color into an `SDL_MessageBoxColor`.
   *
   * \note Message box colors don't feature an alpha value!
   *
   * \return an `SDL_MessageBoxColor` that is equivalent to this color.
   *
   * \since 3.0.0
   */
  [[nodiscard]] explicit constexpr operator SDL_MessageBoxColor() const noexcept
  {
    return {red(), green(), blue()};
  }

  /**
   * \brief Converts the color to `SDL_Color*`.
   *
   * \warning The returned pointer is not to be freed or stored away!
   *
   * \return a pointer to the internal color instance.
   *
   * \since 4.0,0
   */
  [[nodiscard]] explicit operator SDL_Color*() noexcept
  {
    return &m_color;
  }

  /**
   * \brief Converts the color to `const SDL_Color*`.
   *
   * \warning The returned pointer is not to be freed or stored away!
   *
   * \return a pointer to the internal color instance.
   *
   * \since 4.0,0
   */
  [[nodiscard]] explicit operator const SDL_Color*() const noexcept
  {
    return &m_color;
  }

  /// \} End of conversions

  /**
   * \brief Serializes the color.
   *
   * \details This function expects that the archive provides an overloaded `operator()`,
   * used for serializing data. This API is based on the Cereal serialization library.
   *
   * \tparam Archive the type of the archive.
   *
   * \param archive the archive used to serialize the color.
   *
   * \since 5.3.0
   */
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(m_color.r, m_color.g, m_color.b, m_color.a);
  }

  /**
   * \brief Returns a copy of the color with the specified alpha value.
   *
   * \param alpha the alpha component value that will be used by the new color.
   *
   * \return a color that is identical to the color except for the alpha component.
   *
   * \since 5.0.0
   */
  [[nodiscard]] constexpr auto with_alpha(const u8 alpha) const noexcept -> color
  {
    return {red(), green(), blue(), alpha};
  }

  /**
   * \brief Returns the maximum possible value of a color component.
   *
   * \return the maximum possible value of a color component.
   *
   * \since 5.0.0
   */
  [[nodiscard]] constexpr static auto max() noexcept -> u8
  {
    return 0xFF;
  }

 private:
  SDL_Color m_color{0, 0, 0, max()};
};

/**
 * \brief Returns a textual representation of the color.
 *
 * \param color the color that will be converted.
 *
 * \return a textual representation of the color.
 *
 * \since 5.0.0
 */
[[nodiscard]] inline auto to_string(const color& color) -> std::string
{
  return "color{r: " + detail::to_string(color.red()).value() +
         ", g: " + detail::to_string(color.green()).value() +
         ", b: " + detail::to_string(color.blue()).value() +
         ", a: " + detail::to_string(color.alpha()).value() + "}";
}

/**
 * \brief Prints a textual representation of a color.
 *
 * \param stream the stream that will be used.
 * \param color the color that will be printed.
 *
 * \return the used stream.
 *
 * \since 5.0.0
 */
inline auto operator<<(std::ostream& stream, const color& color) -> std::ostream&
{
  return stream << to_string(color);
}

/**
 * \brief Blends two colors according to the specified bias.
 *
 * \pre `bias` should be in the range [0, 1].
 *
 * \details This function applies a linear interpolation for each color component to
 * obtain the blended color. The bias parameter is the "alpha" for the interpolation,
 * which determines how the input colors are blended. For example, a bias of 0 or 1 will
 * simply result in the first or second color being returned, respectively.
 * Subsequently, a bias of 0.5 will blend the two colors evenly.
 *
 * \param a the first color.
 * \param b the second color.
 * \param bias the bias that determines how the colors are blended, in the range [0, 1].
 *
 * \return a color obtained by blending the two supplied colors.
 *
 * \since 6.0.0
 */
[[nodiscard]] inline auto blend(const color& a, const color& b, const double bias = 0.5)
    -> color
{
  assert(bias >= 0);
  assert(bias <= 1.0);

  const auto invBias = 1.0 - bias;

  const auto red = (a.red() * invBias) + (b.red() * bias);
  const auto green = (a.green() * invBias) + (b.green() * bias);
  const auto blue = (a.blue() * invBias) + (b.blue() * bias);
  const auto alpha = (a.alpha() * invBias) + (b.alpha() * bias);

  return color{static_cast<u8>(std::round(red)),
               static_cast<u8>(std::round(green)),
               static_cast<u8>(std::round(blue)),
               static_cast<u8>(std::round(alpha))};
}

/// \name Color comparison operators
/// \{

/**
 * \brief Indicates whether or not the two colors are equal.
 *
 * \param lhs the left-hand side color.
 * \param rhs the right-hand side color.
 *
 * \return `true` if the colors are equal; `false` otherwise.
 *
 * \since 3.0.0
 */
[[nodiscard]] constexpr auto operator==(const color& lhs, const color& rhs) noexcept
    -> bool
{
  return (lhs.red() == rhs.red()) && (lhs.green() == rhs.green()) &&
         (lhs.blue() == rhs.blue()) && (lhs.alpha() == rhs.alpha());
}

/// \copydoc operator==(const color&, const color&)
[[nodiscard]] constexpr auto operator==(const color& lhs, const SDL_Color& rhs) noexcept
    -> bool
{
  return (lhs.red() == rhs.r) && (lhs.green() == rhs.g) && (lhs.blue() == rhs.b) &&
         (lhs.alpha() == rhs.a);
}

/// \copydoc operator==(const color&, const color&)
[[nodiscard]] constexpr auto operator==(const SDL_Color& lhs, const color& rhs) noexcept
    -> bool
{
  return rhs == lhs;
}

/**
 * \copybrief operator==(const color&, const color&)
 *
 * \note The alpha components are not taken into account.
 *
 * \param lhs the left-hand side color.
 * \param rhs the right-hand side color.
 *
 * \return `true` if the colors are equal; `false` otherwise.
 *
 * \since 3.0.0
 */
[[nodiscard]] constexpr auto operator==(const color& lhs,
                                        const SDL_MessageBoxColor& rhs) noexcept -> bool
{
  return (lhs.red() == rhs.r) && (lhs.green() == rhs.g) && (lhs.blue() == rhs.b);
}

/// \copydoc operator==(const color&, const SDL_MessageBoxColor&)
[[nodiscard]] constexpr auto operator==(const SDL_MessageBoxColor& lhs,
                                        const color& rhs) noexcept -> bool
{
  return rhs == lhs;
}

/**
 * \brief Indicates whether or not the two colors aren't equal.
 *
 * \param lhs the left-hand side color.
 * \param rhs the right-hand side color.
 *
 * \return `true` if the colors aren't equal; `false` otherwise.
 *
 * \since 3.0.0
 */
[[nodiscard]] constexpr auto operator!=(const color& lhs, const color& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/// \copydoc operator!=(const color&, const color&)
[[nodiscard]] constexpr auto operator!=(const color& lhs, const SDL_Color& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/// \copydoc operator!=(const color&, const color&)
[[nodiscard]] constexpr auto operator!=(const SDL_Color& lhs, const color& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/**
 * \copybrief operator!=(const color&, const color&)
 *
 * \note The alpha components are not taken into account.
 *
 * \param lhs the left-hand side color.
 * \param rhs the right-hand side color.
 *
 * \return `true` if the colors aren't equal; `false` otherwise.
 *
 * \since 3.0.0
 */
[[nodiscard]] constexpr auto operator!=(const color& lhs,
                                        const SDL_MessageBoxColor& rhs) noexcept -> bool
{
  return !(lhs == rhs);
}

/// \copydoc operator!=(const color&, const SDL_MessageBoxColor&)
[[nodiscard]] constexpr auto operator!=(const SDL_MessageBoxColor& lhs,
                                        const color& rhs) noexcept -> bool
{
  return !(lhs == rhs);
}

/// \} End of color comparison operators

/// \} End of group video

}  // namespace cen

#endif  // CENTURION_COLOR_HEADER
