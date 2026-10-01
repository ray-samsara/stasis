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

#include <map>
#include <string>

namespace stasis
{
  enum class app_mode
  {
    create,
    build
  };
  inline const std::map<app_mode, std::string> app_mode_str_repr = {
      {app_mode::create, "app_mode::create"},
      {app_mode::build, "app_mode::build"}};

  struct app_class_constructor
  {
      std::string argv_0;
      bool creates;
      bool builds;
      std::string dir;
      std::string name;
      std::string output_dir;
  };

  class app
  {
    private:
      std::string execname;
      app_mode mode;
      std::string directory;
      std::string mode_str_repr;
      std::string project_name;
      std::string build_output_directory;

    public:
      // deprecated; soon to be completely replaced
      app(std::string argv_0, bool creates, bool builds, std::string dir,
          std::string name);
      app(app_class_constructor a);
      auto run(void) -> int;
  };
}  // namespace stasis