/*
 * MIT License
 *
 * Copyright (c) 2019 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <string>
#include <memory>
#include <cstdint>
#include <gsl>
#include <SDL_render.h>

namespace centurion {

/**
 * The Image class represents an image that is hardware-accelerated. Instances of the Image class
 * can be implicitly converted to SDL_Texture*.
 *
 * @see SDL_Texture
 * @since 3.0.0
 */
class Image {
 private:
  SDL_Texture* texture = nullptr;

 public:
  /**
   * Creates an image from a pre-existing SDL texture. The created image WILL claim ownership of
   * the supplied pointer!
   *
   * @param texture a pointer to the SDL_Texture that will be claimed, may not be null.
   * @throws NullPointerException if the supplied pointer is null.
   * @since 3.0.0
   */
  explicit Image(gsl::owner<SDL_Texture*> texture);

  /**
   * Creates an image by loading it from a file.
   *
   * @param renderer a pointer to the SDL_Renderer used to create the image, must not be null.
   * @param path the file path of the image that will be loaded.
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  Image(gsl::not_null<SDL_Renderer*> renderer, const std::string& path);

  /**
   * Creates an image that is a copy of the supplied SDL surface.
   *
   * @param renderer the associated renderer instance, may not be null.
   * @param surface the SDL surface that the image will be based on, may not be null
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  Image(gsl::not_null<SDL_Renderer*> renderer, gsl::not_null<SDL_Surface*> surface);

  /**
   * Creates an image by moving the supplied image.
   *
   * @param other the image that will be moved.
   * @since 3.0.0
   */
  Image(Image&& other) noexcept;

  Image(const Image&) noexcept = delete;

  virtual ~Image() noexcept;

  Image& operator=(const Image&) noexcept = delete;

  /**
   * Moves the supplied image into this image.
   *
   * @param other the image that will be moved.
   * @return the changed image.
   * @since 3.0.0
   */
  [[nodiscard]]
  Image& operator=(Image&& other) noexcept;

  /**
   * Creates and returns a unique image from a pre-existing SDL texture. The created image WILL
   * claim ownership of the supplied pointer!
   *
   * @param texture a pointer to the SDL_Texture that will be claimed, may not be null.
   * @returns a unique pointer to the created image.
   * @throws NullPointerException if the supplied pointer is null.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::unique_ptr<Image> unique(gsl::owner<SDL_Texture*> texture);

  /**
   * Creates and returns a unique image by loading it from a file.
   *
   * @param renderer the renderer that will be used when loading the image.
   * @param path the file path of the image.
   * @return a unique pointer to the created image.
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::unique_ptr<Image> unique(gsl::not_null<SDL_Renderer*> renderer,
                                       const std::string& path);

  /**
   * Creates and returns a unique image that is a copy of the supplied SDL surface.
   *
   * @param renderer the associated renderer instance, may not be null.
   * @param surface the SDL surface that the image will be based on, may not be null
   * @return a unique pointer to the created image.
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::unique_ptr<Image> unique(gsl::not_null<SDL_Renderer*> renderer,
                                       gsl::not_null<SDL_Surface*> surface);

  /**
   * Creates and returns a shared image from a pre-existing SDL texture. The created image WILL
   * claim ownership of the supplied pointer!
   *
   * @param texture a pointer to the SDL_Texture that will be claimed, may not be null.
   * @returns a shared pointer to the created image.
   * @throws NullPointerException if the supplied pointer is null.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::shared_ptr<Image> shared(gsl::owner<SDL_Texture*> texture);

  /**
   * Creates and returns a shared image by loading it from a file.
   *
   * @param renderer the renderer that will be used when loading the image.
   * @param path the file path of the image.
   * @return a shared pointer to the created image.
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::shared_ptr<Image> shared(gsl::not_null<SDL_Renderer*> renderer,
                                       const std::string& path);

  /**
   * Creates and returns a shared image that is a copy of the supplied SDL surface.
   *
   * @param renderer the associated renderer instance, may not be null.
   * @param surface the SDL surface that the image will be based on, may not be null
   * @return a shared pointer to the created image.
   * @throws CenturionException if the image cannot be loaded.
   * @since 3.0.0
   */
  [[nodiscard]]
  static std::shared_ptr<Image> shared(gsl::not_null<SDL_Renderer*> renderer,
                                       gsl::not_null<SDL_Surface*> surface);

  /**
   * Sets the alpha value of the image.
   *
   * @param alpha the alpha value, in the range [0, 255].
   * @since 3.0.0
   */
  void set_alpha(uint8_t alpha) noexcept;

  /**
   * Sets the blend mode that will be used by the image.
   *
   * @param mode the blend mode that will be used.
   * @since 3.0.0
   */
  void set_blend_mode(SDL_BlendMode mode) noexcept;

  /**
   * Sets the color modulation of the image. Note, the alpha component in the color struct is
   * ignored by this method.
   *
   * @param color the color that will be used to modulate the color of the image.
   * @since 3.0.0
   */
  void set_color_mod(SDL_Color color) noexcept;

  /**
   * Returns the format of the internal SDL_Texture.
   *
   * @return the format of the internal SDL_Texture.
   * @since 3.0.0
   */
  [[nodiscard]]
  uint32_t get_format() const noexcept;

  /**
   * Returns the texture access of the internal SDL_Texture.
   *
   * @return the texture access of the internal SDL_Texture.
   * @since 3.0.0
   */
  [[nodiscard]]
  int get_access() const noexcept;

  /**
   * Returns the width of the image.
   *
   * @return the width of the image.
   * @since 3.0.0
   */
  [[nodiscard]]
  int get_width() const noexcept;

  /**
   * Returns the height of the image.
   *
   * @return the height of the image.
   * @since 3.0.0
   */
  [[nodiscard]]
  int get_height() const noexcept;

  /**
   * Returns a pointer to the internal SDL_Texture of the image.
   *
   * @return a pointer to the internal SDL_Texture of the image.
   * @since 3.0.0
   */
  [[nodiscard]]
  SDL_Texture* get_texture() noexcept;

  /**
   * Returns a string representation of the image.
   *
   * @return a string representation of the image.
   * @since 3.0.0
   */
  [[nodiscard]]
  std::string to_string() const;

  /**
   * Returns a pointer to the internal SDL_Texture.
   *
   * @return a pointer to the internal SDL_Texture.
   * @since 3.0.0
   */
  /*implicit*/ operator SDL_Texture*() const noexcept;
};

}
