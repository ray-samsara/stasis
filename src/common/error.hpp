#pragma once

#define THROW_ERROR                                                            \
  throw(last_error) { .errnum = errno, .errmsg = strerror(errno) }

// TODO: refactor all instances of this macro so it doesn't conflict with
// windowsslop
#define ERROR(x, msg)                                                          \
  throw(last_error) { .errnum = x, .errmsg = msg }

#include <format>
#include <string>

namespace stasis
{
  struct last_error
  {
      int errnum;
      const char *errmsg;

      auto str(bool pretty = false) -> std::string
      {
        if (pretty)
          return std::format("{{\n\t.errnum = {},\n\t.errmsg = {}\n}}", errnum,
                             errmsg);

        return std::format("{{ .errnum = {}, .errmsg = {} }}", errnum, errmsg);
      }
  };
} // namespace stasis