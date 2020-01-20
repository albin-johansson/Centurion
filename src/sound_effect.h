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

#ifndef CENTURION_NOAUDIO

#pragma once
#include <cstdint>
#include <string>
#include <SDL_mixer.h>
#include "centurion_api.h"

namespace centurion {

/**
 * The SoundEffect class represents sound effects in various file formats.
 *
 * @since 3.0.0
 */
class CENTURION_API SoundEffect {
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
  CENTURION_API void activate(int nLoops) noexcept;

 public:
  /**
   * A constant that can be used as the argument to the SoundEffect::Loop method, in order to make
   * the call more readable.
   *
   * @since 3.0.0
   */
  static const int loopIndefinitely;

  /**
   * @param file the file path of the audio file.
   * @throws CenturionException if the audio file cannot be loaded.
   * @since 3.0.0
   */
  CENTURION_API explicit SoundEffect(const std::string& file);

  /**
   * Creates a sound effect by moving the contents of the supplied sound effect.
   *
   * @param other the sound effect that will be moved.
   * @since 3.0.0
   */
  CENTURION_API SoundEffect(SoundEffect&& other) noexcept;

  SoundEffect(const SoundEffect&) noexcept = delete;

  CENTURION_API virtual ~SoundEffect();

  SoundEffect& operator=(const SoundEffect&) noexcept = delete;

  /**
   * Moves the contents of the supplied sound effect into this one.
   *
   * @param other the sound effect that will be moved.
   * @return the changed sound effect.
   * @since 3.0.0
   */
  [[nodiscard]]
  CENTURION_API SoundEffect& operator=(SoundEffect&& other) noexcept;

  /**
   * Plays the sound effect.
   *
   * @since 3.0.0
   */
  CENTURION_API void play() noexcept;

  /**
   * Loops the sound effect by the specified amount of times.
   *
   * @param nLoops the amount of loops. A negative value indicates that the sound effect should
   * be looped indefinitely.
   * @since 3.0.0
   * @see SoundEffect::loopIndefinitely
   */
  CENTURION_API void loop(int nLoops) noexcept;

  /**
   * Stops the sound effect from playing.
   *
   * @since 3.0.0
   */
  CENTURION_API void stop() noexcept;

  /**
   * Fades in the sound effect. This method has no effect if the supplied duration isn't greater
   * than zero or if the sound effect is currently playing.
   *
   * @param ms the duration to fade in, in milliseconds.
   * @since 3.0.0
   */
  CENTURION_API void fade_in(uint32_t ms) noexcept;

  /**
   * Fades out the sound effect. This method has no effect if the supplied duration isn't greater
   * than zero or if the sound effect isn't currently playing.
   *
   * @param ms the duration to fade in, in milliseconds.
   * @since 3.0.0
   */
  CENTURION_API void fade_out(uint32_t ms) noexcept;

  /**
   * Sets the volume of the sound effect. This method will adjust input values outside
   * the legal range to the closest legal value.
   *
   * @param volume the volume of the sound effect, in the range [0, MIX_MAX_VOLUME].
   * @since 3.0.0
   */
  CENTURION_API void set_volume(int volume) noexcept;

  /**
   * Returns the current volume of the sound effect.
   *
   * @return the current volume of the sound effect.
   * @since 3.0.0
   */
  [[nodiscard]]
  CENTURION_API int get_volume() const noexcept;

  /**
   * Indicates whether or not the sound effect is currently playing.
   *
   * @return true if the sound effect is playing; false otherwise.
   * @since 3.0.0
   */
  [[nodiscard]]
  CENTURION_API bool is_playing() const noexcept;

  CENTURION_API /*implicit*/ operator Mix_Chunk*() const noexcept;
};

}

#endif