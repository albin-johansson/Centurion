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
 * @file to_string.hpp
 *
 * @brief Provides the `to_string` function.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_DETAIL_TO_STRING_HEADER
#define CENTURION_DETAIL_TO_STRING_HEADER

#include <array>         // array
#include <charconv>      // to_chars
#include <optional>      // optional, nullopt
#include <string>        // string
#include <system_error>  // errc

#include "centurion_api.hpp"

namespace centurion::detail {

/**
 * @brief Returns a string representation of an arithmetic value.
 *
 * @tparam T the type of the value that will be converted, must be arithmetic.
 * @tparam bufferSize the size of the stack buffer used, must be big enough
 * to store the characters of the string representation of the value.
 *
 * @param value the value that will be converted.
 *
 * @return a string representation of the supplied value; `std::nullopt` if
 * something goes wrong.
 *
 * @since 5.0.0
 */
template <typename T, std::size_t bufferSize = 10>
[[nodiscard]] auto to_string(T value) -> std::optional<std::string>
{
  std::array<char, bufferSize> buffer{};
  auto first = buffer.data();
  if (const auto [ptr, err] =
          std::to_chars(first, first + buffer.size(), value);
      err == std::errc{}) {
    return std::string{buffer.data(), ptr};
  } else {
    return std::nullopt;
  }
}

}  // namespace centurion::detail

#endif  // CENTURION_DETAIL_TO_STRING_HEADER