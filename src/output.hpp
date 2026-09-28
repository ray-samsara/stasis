#pragma once

#include <cstdio>
#include <format>
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
    int default_callback(rang::fg color, severity severity,
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
      return std::printf("%s", final_text.c_str());
    }

    template <typename... Fmt> int normal(const Fmt... f)
    {
      return default_callback(rang::fg::blue, severity::Normal, f...);
    }

    template <typename... Fmt> int warning(const Fmt... f)
    {
      return default_callback(rang::fg::yellow, severity::Warning, f...);
    }

    template <typename... Fmt> int error(const Fmt... f)
    {
      return default_callback(rang::fg::red, severity::Error, f...);
    }

  } // namespace output
} // namespace stasis