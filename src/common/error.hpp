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

#define THROW_ERROR \
  throw(last_error) { .errnum = errno, .errmsg = strerror(errno) }

#ifdef _WIN32
#define NOGDI
#endif
#define ERROR(x, msg) \
  throw(last_error) { .errnum = x, .errmsg = msg }

#define THROW_ERROR_CPP(e) ERROR(1, e.what())

#include <format>
#include <string>

namespace stasis
{
  struct last_error
  {
      int errnum;
      const char* errmsg;

      auto str(bool pretty = false) -> std::string
      {
        if (pretty)
          return std::format("{{\n\t.errnum = {},\n\t.errmsg = {}\n}}", errnum,
                             errmsg);

        return std::format("{{ .errnum = {}, .errmsg = {} }}", errnum, errmsg);
      }
  };
}  // namespace stasis