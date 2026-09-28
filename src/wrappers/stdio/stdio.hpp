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

      // don't sure why we need these operators but hey they're there, i guess

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