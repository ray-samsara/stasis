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

#include <string>

#include <common/error.hpp>

// vendor
#include <inicpp.h>

// TODO: clang-format isn't formatting the indentation properly (4 spaces
// instead of 2)... FIX IT!
namespace stasis
{
  struct project_info
  {
      std::string name;
      std::string path;
      std::string version;
      std::string author;
      std::string license;

      auto operator==(project_info pi) -> bool
      {
        return this->name == pi.name && this->path == pi.path &&
               this->version == pi.version && this->author == pi.author &&
               this->license == pi.license;
      }

      auto operator!=(project_info pi) -> bool
      {
        return this->name != pi.name && this->path != pi.path &&
               this->version != pi.version && this->author != pi.author &&
               this->license != pi.license;
      }

      constexpr auto empty() const -> bool
      {
        return this->name.empty() && this->path.empty() &&
               this->version.empty() && this->author.empty() &&
               this->license.empty();
      }

      // serialize 'p' into std::string ini
      static auto to(project_info p) -> std::string
      {
        ini::IniFile cfg;
        cfg["stasis_project"]["name"] = p.name;
        cfg["stasis_project"]["path"] = p.path;
        cfg["stasis_project"]["version"] = p.version;
        cfg["stasis_project"]["author"] = p.author;
        cfg["stasis_project"]["license"] = p.license;
        return cfg.encode();
      }

      // deserialize 'cfg' (config CONTENTS, not filepath) to
      // project_info
      static auto from(std::string cfg) -> project_info
      {
        project_info p;
        ini::IniFile cfg_ini;
        cfg_ini.decode(cfg);
        p.name = cfg_ini["stasis_project"]["name"].as<std::string>();
        p.path = cfg_ini["stasis_project"]["path"].as<std::string>();
        p.version = cfg_ini["stasis_project"]["version"].as<std::string>();
        p.author = cfg_ini["stasis_project"]["author"].as<std::string>();
        p.license = cfg_ini["stasis_project"]["license"].as<std::string>();
        return p;
      }
  };

  class project
  {
    private:
      std::string project_path;

    public:
      project_info info;

      // used when reading
      project(std::string filepath);

      // used when writing
      project(project_info data);

      // used to resolve final data;
      project();

      auto empty() -> bool;
      auto exists() -> bool;

      auto operator==(project p) -> bool;
      auto operator!=(project p) -> bool;

      // catch this error with a try/catch block
      auto remove() -> void;
  };
} // namespace stasis
