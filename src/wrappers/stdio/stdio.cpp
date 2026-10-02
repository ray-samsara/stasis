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

#include <cassert>
#include <cstdio>
#include <cstring>

#include "stdio.hpp"
#include <common/error.hpp>
#include <common/logging.hpp>

stasis::file::file(std::string fname)
{
  STASIS_TRACE("[constructor] ---- stasis::file ----");

  // handle is initialized when some other function is called
  handle = NULL;
  filename = fname;
  c_filename = filename.c_str();
}

auto stasis::file::operator==(file f) -> bool
{
  if (this->buffer.empty())
    this->read();

  if (!f.buffer.empty())
    f.read();

  return this->buffer == f.buffer;
}

auto stasis::file::operator!=(file f) -> bool
{
  if (this->buffer.empty())
    this->read();

  if (!f.buffer.empty())
    f.read();

  return this->buffer != f.buffer;
}

auto stasis::file::read() -> void
{
  STASIS_INFO("about to read from file: '{}'", this->filename);

  long length;
  char *c_buffer;
  std::string result;

  STASIS_TRACE("cstdio: fopen()'ing '{}' for reading", this->filename);
  this->handle = std::fopen(this->c_filename, "r");
  if (!this->handle)
    THROW_ERROR;

  std::fseek(this->handle, 0, SEEK_END);
  length = ftell(this->handle);
  std::fseek(this->handle, 0, SEEK_SET);

  c_buffer = new char[length];
  if (!c_buffer)
    THROW_ERROR;

  std::fread(c_buffer, 1, length, this->handle);
  STASIS_WARNING(
      "cstdio: error handling from reading files is not implemented yet");

  /*
  STASIS_INFO("cstdio: clearing out false-negative error indicators");
  std::clearerr(this->handle);

  if (std::ferror(this->handle) != -1)
  {
    STASIS_ERROR("cstdio: ferror() did not return non-zero");
    if (std::feof(this->handle) != -1)
    {
      STASIS_ERROR(
          "cstdio: feof() did not return non-zero; throwing exception");
      ERROR(1, "Unknown error while reading the file");
    }
  }*/

  this->buffer = std::string(c_buffer);
  delete c_buffer;
}

auto stasis::file::write(std::string data) -> void
{
  STASIS_INFO("about to write 'data' of size '{} bytes' to '{}'", data.size(),
              this->filename);

  if (!this->buffer.empty())
  {
    STASIS_WARNING("this->buffer is not empty; clearing it out");
    this->buffer = {};
    assert(this->buffer.empty());
  }

  this->buffer = data;

  if (!this->handle)
  {
    STASIS_TRACE("cstdio: fopen()'ing '{}' for writing", this->filename);
    this->handle = std::fopen(this->c_filename, "w");
    if (!this->handle)
    {
      STASIS_ERROR("cstdio: handle is NULL; trying again for some reason?");
      this->write(data);
    };
  }

  STASIS_TRACE("cstdio: fputs() buffer into handle");
  std::fputs(this->buffer.c_str(), this->handle);
}

stasis::file::~file()
{
  STASIS_INFO("[destructor] ---- stasis::file::~file ----");
  STASIS_TRACE("closing handle that was opened for file '{}'", this->filename);
  if (this->handle)
    std::fclose(this->handle);
}