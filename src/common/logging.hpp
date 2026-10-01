// Copyright (c) 2026 Ray Samsara
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// vendor
#include <rang.hpp>

static auto truncate(std::string str, size_t width, bool show_ellipsis = true)
    -> std::string
{
  if (str.length() > width)
  {
    if (show_ellipsis)
      return str.substr(0, width) + "...";
    else
      return str.substr(0, width);
  }
  return str;
}

namespace stasis
{
  namespace logging
  {
    enum class level
    {
      Trace,
      Info,
      Warning,
      Error,
      NoLog,
    };

    inline const std::vector<std::string> level_str_repr = {
        "trace", "info", "warning", "error", "nil"};

    inline constexpr level default_level = level::Error;
    inline level current_level = default_level;
    inline std::string current_tail;

    template <typename... Fmt>
    auto default_callback(rang::fg color, level lvl,
                          std::string_view function_signature,
                          std::string_view fmt, Fmt&&... f) -> void
    {
      auto msg = std::vformat(fmt, std::make_format_args(f...));

      current_tail = truncate(std::string(function_signature), 35);

      std::ostringstream oss;
      oss << color << level_str_repr[static_cast<int>(lvl)] << rang::fg::reset
          << '(' << rang::style::dim << current_tail << rang::style::reset
          << ')' << ' ' << msg << '\n';

      if (lvl >= current_level)
      {
        std::cerr << oss.str();
      }
    }

    template <typename... Fmt>
    auto trace_impl(std::string_view function_signature, Fmt&&... f) -> void
    {
      default_callback(rang::fg::magenta, level::Trace, function_signature,
                       std::forward<Fmt>(f)...);
    }

    template <typename... Fmt>
    auto info_impl(std::string_view function_signature, Fmt&&... f) -> void
    {
      default_callback(rang::fg::cyan, level::Info, function_signature,
                       std::forward<Fmt>(f)...);
    }

    template <typename... Fmt>
    auto warning_impl(std::string_view function_signature, Fmt&&... f) -> void
    {
      default_callback(rang::fg::yellow, level::Warning, function_signature,
                       std::forward<Fmt>(f)...);
    }

    template <typename... Fmt>
    auto error_impl(std::string_view function_signature, Fmt&&... f) -> void
    {
      default_callback(rang::fg::red, level::Error, function_signature,
                       std::forward<Fmt>(f)...);
    }

  }  // namespace logging
}  // namespace stasis

#define STASIS_TRACE(...) \
  ::stasis::logging::trace_impl(__PRETTY_FUNCTION__, __VA_ARGS__)

#define STASIS_INFO(...) \
  ::stasis::logging::info_impl(__PRETTY_FUNCTION__, __VA_ARGS__)

#define STASIS_WARNING(...) \
  ::stasis::logging::warning_impl(__PRETTY_FUNCTION__, __VA_ARGS__)

#define STASIS_ERROR(...) \
  ::stasis::logging::error_impl(__PRETTY_FUNCTION__, __VA_ARGS__)
