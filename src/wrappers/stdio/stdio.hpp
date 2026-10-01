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

#include <cstdio>
#include <string>

namespace stasis
{

  class file
  {
    private:
      std::FILE *handle;
      std::string filename;
      const char *c_filename;

    public:
      std::string buffer;

      explicit file(std::string fname);

      // TODO: these operators are useless; remove them

      // f.buffer == this->buffer
      //
      // NOTE: operators won't work unless 'handle' has been read from, which in
      // that case the handle is first 'read'.
      auto operator==(file f) -> bool;

      // f.buffer != this->buffer
      //
      // NOTE: operators won't work unless 'handle' has been read from, which in
      // that case the handle is first 'read'.
      auto operator!=(file f) -> bool;

      // reads data from 'handle', to 'buffer'
      //
      // throws an exception (basically errno + strerror on a struct) if it
      // fails
      auto read() -> void;

      // writes data to 'handle' and 'buffer'. overwrites 'buffer' if not empty
      //
      // throws an exception (basically errno + strerror on a struct) if it
      // fails
      auto write(std::string data) -> void;

      // closes 'handle'
      ~file();
  };
} // namespace stasis