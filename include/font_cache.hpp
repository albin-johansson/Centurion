/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file font_cache.hpp
 *
 * @brief Provides the `font_cache` class.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_FONT_CACHE_HEADER
#define CENTURION_FONT_CACHE_HEADER

#include <SDL_ttf.h>

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <utility>

#include "centurion_api.hpp"
#include "centurion_fwd.hpp"
#include "font.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "types.hpp"
#include "unicode_string.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace centurion {

/**
 * @class font_cache
 *
 * @ingroup graphics
 *
 * @brief Provides an API that enables efficient font rendering.
 *
 * @details This class provides two different optimizations.
 *
 * This class can be used to cache glyph textures that can subsequently be used
 * to render strings by simply looking up the individual glyphs and rendering
 * existing textures. It should be noted that the glyph-based rendering will
 * not feature accurate kerning. However, this might not be noticeable and/or
 * worth the performance boost. This type of rendering is *very* efficient for
 * rendering pieces of text that frequently changes, since other approaches
 * would require dynamic allocation and de-allocation for every new rendered
 * string.
 *
 * Furthermore, it's possible to cache full strings and associate them with a
 * user-provided identifier. Using this approach, the strings will be rendered
 * using accurate kerning. The problem is, as you might guess, is that it's hard
 * to know the exact strings you will render at compile-time. Use this option
 * if you know that you're going to render some specific string a lot.
 *
 * @since 5.0.0
 *
 * @headerfile font_cache.hpp
 */
class font_cache final
{
 public:
  /**
   * @typedef uptr
   *
   * @brief Simple alias for a unique pointer to a font cache instance.
   *
   * @since 5.0.0
   */
  using uptr = std::unique_ptr<font_cache>;

  /**
   * @typedef sptr
   *
   * @brief Simple alias for a shared pointer to a font cache instance.
   *
   * @since 5.0.0
   */
  using sptr = std::shared_ptr<font_cache>;

  /**
   * @typedef wptr
   *
   * @brief Simple alias for a weak pointer to a font cache instance.
   *
   * @since 5.0.0
   */
  using wptr = std::weak_ptr<font_cache>;

  /**
   * @struct glyph_data
   *
   * @brief Simple aggregate that contains a texture and metrics for a glyph.
   *
   * @since 5.0.0
   *
   * @headerfile font_cache.hpp
   */
  struct glyph_data final
  {
    texture cached;         ///< The cached texture.
    glyph_metrics metrics;  ///< The metrics of the glyph.
  };

  /**
   * @brief Creates an empty font cache instance.
   *
   * @note You have to explicitly state what glyphs that you want to be cached.
   *
   * @param font the font that will be used.
   *
   * @since 5.0.0
   */
  CENTURION_API
  explicit font_cache(font&& font) noexcept;

  /**
   * @brief Creates an empty font cache, and creates the associated font
   * in-place.
   *
   * @tparam Args the types of the arguments forwarded to the font constructor.
   *
   * @param args the arguments that will be forwarded to the font constructor.
   *
   * @since 5.0.0
   */
  template <typename... Args>
  explicit font_cache(Args&&... args);

  /**
   * @copydoc font_cache(font&&)
   */
  CENTURION_QUERY
  static auto unique(font&& font) -> uptr;

  /**
   * @brief Creates and returns a unique pointer to a font cache instance.
   *
   * @details Creates an empty font cache, and creates the associated font
   * in-place.
   *
   * @tparam Args the types of the arguments forwarded to the font constructor.
   *
   * @param args the arguments that will be forwarded to the font constructor.
   *
   * @return a unique pointer to a font cache.
   *
   * @since 5.0.0
   */
  template <typename... Args>
  [[nodiscard]] static auto unique(Args&&... args) -> uptr;

  /**
   * @copydoc font_cache(font&&)
   */
  CENTURION_QUERY
  static auto shared(font&& font) -> sptr;

  /**
   * @brief Creates and returns a shared pointer to a font cache instance.
   *
   * @details Creates an empty font cache, and creates the associated font
   * in-place.
   *
   * @tparam Args the types of the arguments forwarded to the font constructor.
   *
   * @param args the arguments that will be forwarded to the font constructor.
   *
   * @return a shared pointer to a font cache.
   *
   * @since 5.0.0
   */
  template <typename... Args>
  [[nodiscard]] static auto shared(Args&&... args) -> sptr;

  /// @name String caching
  /// @brief Methods related to caching strings as textures.
  /// @{

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_blended_utf8`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_utf8(hash_id id, nn_czstring string, Renderer& renderer);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param wrap the width in pixels after which the text will be wrapped.
   *
   * @see `basic_renderer::render_blended_wrapped_utf8`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_wrapped_utf8(hash_id id,
                                  nn_czstring string,
                                  Renderer& renderer,
                                  u32 wrap);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param background the color used for the background box.
   *
   * @see `basic_renderer::render_shaded_utf8`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_shaded_utf8(hash_id id,
                         nn_czstring string,
                         Renderer& renderer,
                         const color& background);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_solid_utf8`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_solid_utf8(hash_id id, nn_czstring string, Renderer& renderer);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_blended_latin1`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_latin1(hash_id id, nn_czstring string, Renderer& renderer);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param wrap the width in pixels after which the text will be wrapped.
   *
   * @see `basic_renderer::render_blended_wrapped_latin1`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_wrapped_latin1(hash_id id,
                                    nn_czstring string,
                                    Renderer& renderer,
                                    u32 wrap);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param background the color used for the background box.
   *
   * @see `basic_renderer::render_shaded_latin1`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_shaded_latin1(hash_id id,
                           nn_czstring string,
                           Renderer& renderer,
                           const color& background);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_solid_latin1`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_solid_latin1(hash_id id, nn_czstring string, Renderer& renderer);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_blended_unicode`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_unicode(hash_id id,
                             const unicode_string& string,
                             Renderer& renderer);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param wrap the width in pixels after which the text will be wrapped.
   *
   * @see `basic_renderer::render_blended_wrapped_unicode`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_blended_wrapped_unicode(hash_id id,
                                     const unicode_string& string,
                                     Renderer& renderer,
                                     u32 wrap);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   * @param background the color used for the background box.
   *
   * @see `basic_renderer::render_shaded_unicode`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_shaded_unicode(hash_id id,
                            const unicode_string& string,
                            Renderer& renderer,
                            const color& background);

  /**
   * @brief Caches the supplied string by rendering it to a texture.
   *
   * @details This function respects the kerning of the font. Any previous
   * cached string associated with the supplied ID will be overwritten.
   *
   * @tparam Renderer the type of the renderer that will be used.
   *
   * @param id the identifier that will be associated with the texture.
   * @param string the string that will be cached.
   * @param renderer the renderer that will be used to create the string
   * texture.
   *
   * @see `basic_renderer::render_solid_unicode`
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void store_solid_unicode(hash_id id,
                           const unicode_string& string,
                           Renderer& renderer);

  /**
   * @brief Indicates whether or not there is a cached string texture associated
   * with the specified key.
   *
   * @param id the key that will be checked.
   *
   * @return `true` if there is a cached texture associated with the key;
   * `false` otherwise.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto has_stored(hash_id id) const noexcept -> bool
  {
    return m_strings.find(id) != m_strings.end();
  }

  /**
   * @brief Returns the cached texture associated with the specified ID.
   *
   * @pre `id` **must** be associated with a cached string texture.
   *
   * @param id the key of the cached texture to obtain.
   *
   * @return the cached texture associated with the key.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto get_stored(hash_id id) const -> const texture&
  {
    return m_strings.at(id);
  }

  /**
   * @brief Returns a pointer to the texture associated with the specified key.
   *
   * @note The returned pointer is not suitable for storing for longer than
   * absolutely necessary, as it might get invalidated by modifications of
   * the font cache.
   *
   * @param id the key of the desired texture.
   *
   * @return a pointer to the texture associated with the specified key;
   * `nullptr` if no texture is found.
   *
   * @since 5.0.0
   */
  CENTURION_QUERY
  auto try_get_stored(hash_id id) const noexcept -> const texture*;

  /// @}  // end of string caching

  /// @name Glyph caching
  /// @brief Methods related to cached Unicode glyph textures.
  /// @{

  /**
   * @brief Adds a glyph to the font cache.
   *
   * @details This method has no effect if the supplied glyph isn't provided
   * by the associated font, or if the supplied glyph has already been cached.
   *
   * @tparam Renderer the type of the renderer.
   *
   * @param renderer the renderer that will be used to create the glyph texture.
   * @param glyph the glyph that will be cached.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void add_glyph(Renderer& renderer, unicode glyph);

  /**
   * @brief Caches the glyphs in the specified range.
   *
   * @details The range is interpreted as [min, max), i.e. the the `min`
   * value is included, and `max` is excluded.
   *
   * @remark For an overview of the various Unicode blocks, see <a
   * href="https://unicode-table.com/en/blocks/">this</a>.
   *
   * @tparam Renderer the type of the renderer.
   *
   * @param renderer the renderer that will be used to create the glyph
   * textures.
   * @param begin the first glyph that will be included.
   * @param end the "end" glyph in the range, will not be included.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void add_range(Renderer& renderer, unicode begin, unicode end);

  /**
   * @brief Attempts to cache all printable basic latin characters.
   *
   * @details The basic latin set provides the most common characters, such as
   * upper- and lower-case latin letters, numbers and symbols. This method
   * might throw if something goes wrong when creating the textures.
   *
   * @tparam Renderer the type of the renderer.
   *
   * @param renderer the renderer that will be used to create the glyph
   * textures.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void add_basic_latin(Renderer& renderer);

  /**
   * @brief Attempts to cache all printable Latin-1 supplement characters.
   *
   * @tparam Renderer the type of the renderer.
   *
   * @param renderer the renderer that will be used to create the glyph
   * textures.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void add_latin1_supplement(Renderer& renderer);

  /**
   * @brief Attempts to cache all printable Latin-1 characters.
   *
   * @note This method is effectively equivalent to calling both
   * `add_basic_latin` and `add_latin1_supplement`.
   *
   * @tparam Renderer the type of the renderer.
   *
   * @param renderer the renderer that will be used to create the glyph
   * textures.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  void add_latin1(Renderer& renderer);

  /**
   * @brief Indicates whether or not the specified glyph has been cached.
   *
   * @param glyph the glyph to check.
   *
   * @return `true` if the specified glyph has been cached; `false` otherwise.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto has(unicode glyph) const noexcept -> bool
  {
    return m_glyphs.count(glyph);
  }

  /**
   * @brief Returns the data associated with the specified glyph.
   *
   * @pre `glyph` **must** have been previously cached.
   *
   * @details The recommended way to use this method is with structured
   * bindings, as in the following example.
   * @code{.cpp}
   *   cen::font_cache cache = ...;
   *   const auto& [cachedTexture, glyphMetrics] = cache.at('A'_uni);
   * @endcode
   *
   * @param glyph the desired glyph to lookup the data for.
   *
   * @return the cached texture and metrics associated with the glyph.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto at(unicode glyph) const -> const glyph_data&
  {
    return m_glyphs.at(glyph);
  }

  /**
   * @brief Returns the data associated with the specified glyph.
   *
   * @note This function is equivalent to calling `at`.
   *
   * @pre `glyph` **must** have been previously cached.
   *
   * @details The recommended way to use this method is with structured
   * bindings, as in the following example.
   * @code{.cpp}
   *   cen::font_cache cache = ...;
   *   const auto& [cachedTexture, glyphMetrics] = cache['A'];
   * @endcode
   *
   * @param glyph the desired glyph to lookup the data for.
   *
   * @return the cached texture and metrics associated with the glyph.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto operator[](unicode glyph) const -> const glyph_data&
  {
    return at(glyph);
  }

  ///@}  // end of glyph caching

  /**
   * @brief Returns the font used by the cache.
   *
   * @return a reference to the internal font.
   *
   * @since 5.0.0
   */
  [[nodiscard]] auto get_font() noexcept -> font& { return m_font; }

  /**
   * @copydoc get_font
   */
  [[nodiscard]] auto get_font() const noexcept -> const font& { return m_font; }

 private:
  font m_font;
  std::unordered_map<unicode, glyph_data> m_glyphs{};
  std::unordered_map<hash_id, texture> m_strings{};

  /**
   * @brief Creates and returns a texture for the specified glyph.
   *
   * @details The glyph is rendered with `TTF_RenderGlyph_Blended`.
   *
   * @param renderer the renderer that will be used.
   * @param glyph the Unicode glyph that will be rendered.
   *
   * @return a texture that represents the specified glyph.
   *
   * @since 5.0.0
   */
  template <typename Renderer>
  [[nodiscard]] auto create_glyph_texture(Renderer& renderer, unicode glyph)
      -> texture;

  CENTURION_API
  void store(hash_id id, texture&& texture);
};

template <typename... Args>
font_cache::font_cache(Args&&... args) : m_font{std::forward<Args>(args)...}
{}

template <typename... Args>
auto font_cache::unique(Args&&... args) -> uptr
{
  return std::make_unique<font_cache>(std::forward<Args>(args)...);
}

template <typename... Args>
auto font_cache::shared(Args&&... args) -> sptr
{
  return std::make_shared<font_cache>(std::forward<Args>(args)...);
}

template <typename Renderer>
void font_cache::store_blended_utf8(hash_id id,
                                    nn_czstring string,
                                    Renderer& renderer)
{
  store(id, renderer.render_blended_utf8(string, get_font()));
}

template <typename Renderer>
void font_cache::store_blended_wrapped_utf8(hash_id id,
                                            nn_czstring string,
                                            Renderer& renderer,
                                            u32 wrap)
{
  store(id, renderer.render_blended_wrapped_utf8(string, get_font(), wrap));
}

template <typename Renderer>
void font_cache::store_shaded_utf8(hash_id id,
                                   nn_czstring string,
                                   Renderer& renderer,
                                   const color& background)
{
  store(id, renderer.render_shaded_utf8(string, get_font(), background));
}

template <typename Renderer>
void font_cache::store_solid_utf8(hash_id id,
                                  nn_czstring string,
                                  Renderer& renderer)
{
  store(id, renderer.render_solid_utf8(string, get_font()));
}

template <typename Renderer>
void font_cache::store_blended_latin1(hash_id id,
                                      nn_czstring string,
                                      Renderer& renderer)
{
  store(id, renderer.render_blended_latin1(string, get_font()));
}

template <typename Renderer>
void font_cache::store_blended_wrapped_latin1(hash_id id,
                                              nn_czstring string,
                                              Renderer& renderer,
                                              u32 wrap)
{
  store(id, renderer.render_blended_wrapped_latin1(string, get_font(), wrap));
}

template <typename Renderer>
void font_cache::store_shaded_latin1(hash_id id,
                                     nn_czstring string,
                                     Renderer& renderer,
                                     const color& background)
{
  store(id, renderer.render_shaded_latin1(string, get_font(), background));
}

template <typename Renderer>
void font_cache::store_solid_latin1(hash_id id,
                                    nn_czstring string,
                                    Renderer& renderer)
{
  store(id, renderer.render_solid_latin1(string, get_font()));
}

template <typename Renderer>
void font_cache::store_blended_unicode(hash_id id,
                                       const unicode_string& string,
                                       Renderer& renderer)
{
  store(id, renderer.render_blended_unicode(string, get_font()));
}

template <typename Renderer>
void font_cache::store_blended_wrapped_unicode(hash_id id,
                                               const unicode_string& string,
                                               Renderer& renderer,
                                               u32 wrap)
{
  store(id, renderer.render_blended_wrapped_unicode(string, get_font(), wrap));
}

template <typename Renderer>
void font_cache::store_shaded_unicode(hash_id id,
                                      const unicode_string& string,
                                      Renderer& renderer,
                                      const color& background)
{
  store(id, renderer.render_shaded_unicode(string, get_font(), background));
}

template <typename Renderer>
void font_cache::store_solid_unicode(hash_id id,
                                     const unicode_string& string,
                                     Renderer& renderer)
{
  store(id, renderer.render_solid_unicode(string, get_font()));
}

template <typename Renderer>
void font_cache::add_glyph(Renderer& renderer, unicode glyph)
{
  if (has(glyph) || !m_font.is_glyph_provided(glyph)) {
    return;
  }

  m_glyphs.emplace(glyph,
                   glyph_data{create_glyph_texture(renderer, glyph),
                              m_font.get_metrics(glyph).value()});
}

template <typename Renderer>
auto font_cache::create_glyph_texture(Renderer& renderer, unicode glyph)
    -> texture
{
  const surface surf{TTF_RenderGlyph_Blended(
      m_font.get(), glyph, static_cast<SDL_Color>(renderer.get_color()))};
  return texture{renderer, surf};
}

template <typename Renderer>
void font_cache::add_range(Renderer& renderer, unicode begin, unicode end)
{
  for (auto ch = begin; ch < end; ++ch) {
    add_glyph(renderer, ch);
  }
}

template <typename Renderer>
void font_cache::add_basic_latin(Renderer& renderer)
{
  // https://unicode-table.com/en/blocks/basic-latin/
  add_range(renderer, 0x20, 0x7F);
}

template <typename Renderer>
void font_cache::add_latin1_supplement(Renderer& renderer)
{
  // https://unicode-table.com/en/blocks/latin-1-supplement/
  add_range(renderer, 0xA0, 0x100);
}

template <typename Renderer>
void font_cache::add_latin1(Renderer& renderer)
{
  add_basic_latin(renderer);
  add_latin1_supplement(renderer);
}

}  // namespace centurion

#endif  // CENTURION_FONT_CACHE_HEADER