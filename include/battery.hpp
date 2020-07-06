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
 * @brief Provides battery related utilities.
 *
 * @file battery.hpp
 * @author Albin Johansson
 * @date 2019-2020
 * @copyright MIT License
 */

#ifndef CENTURION_BATTERY_HEADER
#define CENTURION_BATTERY_HEADER

#include <SDL_power.h>

#include "centurion_api.hpp"
#include "centurion_utils.hpp"

/**
 * @namespace centurion::battery
 *
 * @brief Contains utilities related to the battery of the system.
 *
 * @since 5.0.0
 */
namespace centurion::battery {

/**
 * @enum PowerState
 * @brief Mirrors the values of the `SDL_PowerState` enum.
 *
 * @since 3.0.0
 *
 * @var PowerState::Unknown
 * The status is unknown.
 * @var PowerState::OnBattery
 * Not plugged in and running on battery.
 * @var PowerState::NoBattery
 * No battery available.
 * @var PowerState::Charging
 * Currently charging the battery.
 * @var PowerState::Charged
 * Currently plugged in and charged.
 *
 * @headerfile battery.hpp
 */
enum class PowerState {
  Unknown = SDL_POWERSTATE_UNKNOWN,
  OnBattery = SDL_POWERSTATE_ON_BATTERY,
  NoBattery = SDL_POWERSTATE_NO_BATTERY,
  Charging = SDL_POWERSTATE_CHARGING,
  Charged = SDL_POWERSTATE_CHARGED
};

/**
 * @brief Indicates whether or not two power states values are the same.
 *
 * @param lhs the left-hand side power state value.
 * @param rhs the right-hand side power state value.
 *
 * @return `true` if the power states are the same; `false` otherwise.
 *
 * @since 3.0.0
 */
[[nodiscard]] inline auto operator==(PowerState lhs,
                                     SDL_PowerState rhs) noexcept -> bool
{
  return static_cast<SDL_PowerState>(lhs) == rhs;
}

/**
 * @copydoc operator==(PowerState, SDL_PowerState)
 */
[[nodiscard]] inline auto operator==(SDL_PowerState lhs,
                                     PowerState rhs) noexcept -> bool
{
  return rhs == lhs;
}

/**
 * @brief Returns the seconds of battery life that is remaining.
 *
 * @return seconds of remaining battery life; `nothing` if the value cannot
 * be computed.
 *
 * @since 3.0.0
 */
CENTURION_QUERY auto seconds_left() noexcept -> Optional<int>;

/**
 * @brief Returns the amount of minutes of battery life that is remaining.
 *
 * @return minutes of remaining battery life; `nothing` if the value cannot
 * be computed.
 *
 * @since 3.0.0
 */
CENTURION_QUERY auto minutes_left() noexcept -> Optional<int>;

/**
 * @brief Returns the percentage of battery life that is currently left.
 *
 * @return percentage of remaining battery life, in the range [0, 100];
 * `nothing` if the battery percentage isn't available.
 *
 * @since 3.0.0
 */
CENTURION_QUERY auto percentage() noexcept -> Optional<int>;

/**
 * @brief Returns the current power state.
 *
 * @return the current power state.
 *
 * @since 3.0.0
 */
CENTURION_QUERY auto state() noexcept -> PowerState;

/**
 * @brief Indicates whether or not the system is running on a battery.
 *
 * @details This method is simply a convenience method that is based on the
 * `battery::state()` method.
 *
 * @return `true` if the system is running on a battery; `false` otherwise.
 *
 * @since 4.0.0
 */
CENTURION_QUERY auto exists() noexcept -> bool;

}  // namespace centurion::battery

#ifdef CENTURION_HEADER_ONLY
#include "battery.cpp"
#endif

#endif  // CENTURION_BATTERY_HEADER