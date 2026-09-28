#include <cerrno>
#include <cstdio>
#include <cstring>

#include "stdio.hpp"
#include <common/error.hpp>

stasis::file::file(std::string fname)
{
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
  long length;
  char *c_buffer;
  std::string result;
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
  if (ferror(this->handle))
  {
    if (!feof(this->handle))
      ERROR(1, "Unknown error while reading the file");
  }

  this->buffer = std::string(c_buffer);
  delete c_buffer;
}

auto stasis::file::write(std::string data) -> void
{
  if (!this->buffer.empty())
    this->buffer = {};

  this->buffer = data;

  if (!this->handle)
  {
    this->handle = std::fopen(this->c_filename, "w");
    if (!this->handle)
      this->write(data);
  }

  std::fputs(this->buffer.c_str(), this->handle);
  std::fputc('\0', this->handle);
}

stasis::file::~file()
{
  if (this->handle)
    std::fclose(this->handle);
}