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
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

// vendor
#include <rang.hpp>

namespace stasis
{
  namespace output
  {

    enum class severity
    {
      Normal,
      Warning,
      Error
    };

    inline const std::map<severity, std::string> Severities = {
        {severity::Normal, "  ~ "},
        {severity::Warning, "  ! "},
        {severity::Error, " !!!"}};

    template <typename... Fmt>
    void default_callback(rang::fg color, severity severity,
                          std::string_view fmt, Fmt &&...f)
    {
      auto msg =
          std::vformat(fmt, std::make_format_args(std::forward<Fmt>(f)...));

      std::ostringstream oss;
      oss << ' ' << color << Severities.at(severity) << rang::style::reset
          << ' ' << msg << std::endl;

      // auto final_text = std::format(" {}{}{} {}\n", color,
      // Severities.at(severity),
      //                              rang::style::reset, msg);

      auto final_text = oss.str();
      std::cerr << final_text;
    }

    template <typename... Fmt> void normal(const Fmt... f)
    {
      default_callback(rang::fg::blue, severity::Normal, f...);
    }

    template <typename... Fmt> void warning(const Fmt... f)
    {
      default_callback(rang::fg::yellow, severity::Warning, f...);
    }

    template <typename... Fmt> void error(const Fmt... f)
    {
      default_callback(rang::fg::red, severity::Error, f...);
    }

  } // namespace output
} // namespace stasis