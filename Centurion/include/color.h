#pragma once
#include <SDL_config.h>
#include <SDL_pixels.h>
#include <memory>

namespace centurion {
namespace visuals {

/**
\brief The Color class simply represents a color.
*/
class Color {
 private:
  SDL_Color color;

 public:
  /**
  \brief The highest possible value for the red, green, blue and alpha
  components.
  */
  const static uint8_t MAX_VAL = 255;

  /**
  \brief The lowest possible value for the red, green, blue and alpha
  components.
  */
  const static uint8_t MIN_VAL = 0;

  /**
  \brief A constant that represents the color red.
  */
  const static Color RED;

  /**
  \brief A constant that represents the color green.
  */
  const static Color GREEN;

  /**
  \brief A constant that represents the color blue.
  */
  const static Color BLUE;

  /**
  \brief A constant that represents the color white.
  */
  const static Color WHITE;

  /**
  \brief A constant that represents the color black.
  */
  const static Color BLACK;

  /**
  \brief A constant that represents the color gray.
  */
  const static Color GRAY;

  /**
  \param r - the red component of the color in the range [MIN_VAL, MAX_VAL].
  \param g - the green component of the color in the range [MIN_VAL, MAX_VAL].
  \param b - the blue component of the color in the range [MIN_VAL, MAX_VAL].
  \param a - the alpha component of the color in the range [MIN_VAL, MAX_VAL].
  */
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

  ~Color();

  /**
  \brief Returns the red component of this color.
  */
  inline uint8_t GetRed() const noexcept { return color.r; }

  /**
  \brief Returns the green component of this color.
  */
  inline uint8_t GetGreen() const noexcept { return color.g; }

  /**
  \brief Returns the blue component of this color.
  */
  inline uint8_t GetBlue() const noexcept { return color.b; }

  /**
  \brief Returns the alpha component of this color.
  */
  inline uint8_t GetAlpha() const noexcept { return color.a; }

  /**
  \brief Returns an SDL_Color instance that represents this Color.
  */
  inline SDL_Color GetSDLVersion() const noexcept { return color; }
};

typedef std::shared_ptr<centurion::visuals::Color> Color_sptr;
typedef std::unique_ptr<centurion::visuals::Color> Color_uptr;
typedef std::weak_ptr<centurion::visuals::Color> Color_wptr;

}  // namespace visuals
}  // namespace centurion