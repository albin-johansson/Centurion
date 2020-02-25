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

#ifndef CENTURION_SOUND_EFFECT_HEADER
#define CENTURION_SOUND_EFFECT_HEADER

#ifndef CENTURION_NOAUDIO

#include <SDL_mixer.h>

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include "centurion_api.h"

namespace centurion {
namespace audio {

/**
 * The SoundEffect class represents sound effects in various file formats.
 *
 * @since 3.0.0
 */
class SoundEffect final {
 private:
  static constexpr int undefinedChannel = -1;

  Mix_Chunk* chunk = nullptr;
  int channel = undefinedChannel;

  /**
   * Activates the sound effect by playing it the specified amount of times.
   *
   * @param nLoops the amount of times to play the sound effect.
   * @since 3.0.0
   */
  CENTURION_API
  void activate(int nLoops) noexcept;

 public:
  /**
   * @param file the file path of the audio file.
   * @throws CenturionException if the audio file cannot be loaded.
   * @since 3.0.0
   */
  CENTURION_API
  explicit SoundEffect(const std::string& file);

  /**
   * Creates a sound effect by moving the contents of the supplied sound effect.
   *
   * @param other the sound effect that will be moved.
   * @since 3.0.0
   */
  CENTURION_API
  SoundEffect(SoundEffect&& other) noexcept;

  SoundEffect(const SoundEffect&) noexcept = delete;

  CENTURION_API
  ~SoundEffect();

  SoundEffect& operator=(const SoundEffect&) noexcept = delete;

  /**
   * Moves the contents of the supplied sound effect into this one.
   *
   * @param other the sound effect that will be moved.
   * @return the changed sound effect.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  SoundEffect& operator=(SoundEffect&& other) noexcept;

  /**
   * Creates and returns a unique pointer to a SoundEffect instance.
   *
   * @param file the file path of the audio file.
   * @return a unique pointer to a SoundEffect instance.
   * @throws CenturionException if the audio file cannot be loaded.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  static std::unique_ptr<SoundEffect> unique(const std::string& file);

  /**
   * Creates and returns a shared pointer to a SoundEffect instance.
   *
   * @param file the file path of the audio file.
   * @return a shared pointer to a SoundEffect instance.
   * @throws CenturionException if the audio file cannot be loaded.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  static std::shared_ptr<SoundEffect> shared(const std::string& file);

  /**
   * Plays the sound effect.
   *
   * @param nLoops the amount of loops. A negative value indicates that the
   * sound effect should be looped forever.
   * @since 3.0.0
   */
  CENTURION_API
  void play(int nLoops = 0) noexcept;

  /**
   * Stops the sound effect from playing.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void stop() noexcept;

  /**
   * Fades in the sound effect. This method has no effect if the supplied
   * duration isn't greater than zero or if the sound effect is currently
   * playing.
   *
   * @param ms the duration to fade in, in milliseconds.
   * @since 3.0.0
   */
  CENTURION_API
  void fade_in(int ms) noexcept;

  /**
   * Fades out the sound effect. This method has no effect if the supplied
   * duration isn't greater than zero or if the sound effect isn't currently
   * playing.
   *
   * @param ms the duration to fade in, in milliseconds.
   * @since 3.0.0
   */
  CENTURION_API
  void fade_out(int ms) noexcept;

  /**
   * Sets the volume of the sound effect. This method will adjust input values
   * outside the legal range to the closest legal value.
   *
   * @param volume the volume of the sound effect, in the range [0, maxVolume].
   * @since 3.0.0
   */
  CENTURION_API
  void set_volume(int volume) noexcept;

  /**
   * Returns the current volume of the sound effect. By default, this property
   * is set to 128.
   *
   * @return the current volume of the sound effect.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  int get_volume() const noexcept;

  /**
   * Indicates whether or not the sound effect is currently playing.
   *
   * @return true if the sound effect is playing; false otherwise.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  bool is_playing() const noexcept;

  /**
   * Returns a textual representation of the sound effect.
   *
   * @return a textual representation of the sound effect.
   * @since 3.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  std::string to_string() const;

  /**
   * Converts to a Mix_Chunk pointer.
   *
   * @return a pointer to the internal Mix_Chunk instance.
   * @since 3.0.0
   */
  CENTURION_API
  /*implicit*/ operator Mix_Chunk*() const noexcept;

  /**
   * Returns the maximum possible volume value.
   *
   * @return the maximum possible volume value.
   * @since 3.1.0
   */
  CENTURION_NODISCARD
  static constexpr int get_max_volume() noexcept
  {
    return MIX_MAX_VOLUME;
  }
};

#ifdef CENTURION_HAS_IS_FINAL_TYPE_TRAIT
static_assert(std::is_final<SoundEffect>::value, "SoundEffect isn't final!");
#endif

static_assert(std::is_nothrow_move_constructible<SoundEffect>::value,
              "SoundEffect isn't nothrow move constructible!");

static_assert(std::is_nothrow_move_assignable<SoundEffect>::value,
              "SoundEffect isn't nothrow move assignable!");

static_assert(!std::is_copy_constructible<SoundEffect>::value,
              "SoundEffect is copy constructible!");

static_assert(!std::is_copy_assignable<SoundEffect>::value,
              "SoundEffect is copy assignable!");

static_assert(std::is_convertible<SoundEffect, Mix_Chunk*>::value,
              "SoundEffect isn't convertible to Mix_Chunk*!");

}  // namespace audio
}  // namespace centurion

#ifdef CENTURION_HEADER_ONLY
#include "sound_effect.cpp"
#endif

#endif  // CENTURION_NOAUDIO
#endif  // CENTURION_SOUND_EFFECT_HEADER