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
 * @file key_state.hpp
 *
 * @brief Provides the `key_state` class.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_KEY_STATE_HEADER
#define CENTURION_KEY_STATE_HEADER

#include <SDL.h>

#include <array>

#include "centurion_api.hpp"
#include "centurion_utils.hpp"
#include "key.hpp"
#include "key_modifier.hpp"

/**
 * @namespace centurion::input
 *
 * @brief Contains components related to input state.
 *
 * @details This namespace provides components that are an alternative to the
 * traditional events.
 *
 * @see `key_state`
 * @see `mouse_state`
 */
namespace centurion::input {

/**
 * @class key_state
 *
 * @brief Provides information about the keyboard state.
 *
 * @details Using the keyboard state is an alternative to using events for
 * keyboard input.
 *
 * @since 3.0.0
 *
 * @headerfile key_state.hpp
 */
class key_state final {
 public:
  /**
   * @brief Creates a `key_state` instance.
   *
   * @since 3.0.0
   */
  CENTURION_API
  key_state() noexcept;

  /**
   * @copydoc key_state()
   */
  CENTURION_QUERY
  static auto unique() -> std::unique_ptr<key_state>;

  /**
   * @copydoc key_state()
   */
  CENTURION_QUERY
  static auto shared() -> std::shared_ptr<key_state>;

  /**
   * @brief Updates the state of the key state object.
   *
   * @note `SDL_PumpEvents` isn't invoked by this method.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void update() noexcept;

  /**
   * @brief Indicates whether or not the specified key is being pressed.
   *
   * @details This method returns false if the supplied key isn't recognized.
   *
   * @param key the key that will be checked.
   *
   * @return `true` if the key is being pressed; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto is_pressed(const Key& key) const noexcept -> bool;

  /**
   * @brief Indicates whether or not the specified key has been pressed during
   * more than one update of the key state.
   *
   * @details This method returns false if the supplied key isn't recognized.
   *
   * @param key the key that will be checked.
   *
   * @return `true` if the key has been held down; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto is_held(const Key& key) const noexcept -> bool;

  /**
   * @brief Indicates whether or not a key just became pressed in the last
   * update of the key state.
   *
   * @details This method returns false if the supplied key isn't recognized.
   *
   * @param key the key that will be checked.
   *
   * @return `true` if the key has just been pressed; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto was_just_pressed(const Key& key) const noexcept -> bool;

  /**
   * @brief Indicates whether or not the specified key was released in the last
   * update of the key state.
   *
   * @details This method returns false if the supplied key isn't recognized.
   *
   * @param key the key that will be checked.
   *
   * @return `true` if the key was released; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto was_just_released(const Key& key) const noexcept -> bool;

  /**
   * @brief Indicates whether or not the specified key modifier is active.
   *
   * @note Multiple key modifiers can be active at the same time.
   *
   * @param modifier the key modifier that will be checked.
   *
   * @return `true` if the specified key modifier is active; `false` otherwise.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto modifier_active(key_modifier modifier) const noexcept -> bool;

  /**
   * @brief Returns the total amount of keys.
   *
   * @return the total amount of keys.
   *
   * @since 3.0.0
   */
  [[nodiscard]] auto amount_of_keys() const noexcept -> int { return m_nKeys; }

 private:
  const u8* m_states{nullptr};
  std::array<u8, static_cast<int>(SDL_NUM_SCANCODES)> m_previousStates;
  int m_nKeys{};
};

static_assert(std::is_final_v<key_state>);
static_assert(std::is_default_constructible_v<key_state>);
static_assert(std::is_nothrow_destructible_v<key_state>);
static_assert(std::is_nothrow_move_constructible_v<key_state>);
static_assert(std::is_nothrow_move_assignable_v<key_state>);
static_assert(std::is_nothrow_copy_constructible_v<key_state>);
static_assert(std::is_nothrow_copy_assignable_v<key_state>);

}  // namespace centurion::input

#ifdef CENTURION_HEADER_ONLY
#include "key_state.cpp"
#endif

#endif  // CENTURION_KEY_STATE_HEADER
