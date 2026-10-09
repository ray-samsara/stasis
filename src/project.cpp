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

#include "project.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
#define NOGDI
#endif
#include <common/error.hpp>
#include <common/logging.hpp>
#include <common/project_info_name.hpp>

namespace fs = std::filesystem;

// used when reading
stasis::project::project(std::string filepath)
{
  STASIS_TRACE("[constructor] ---- stasis::project ---- (reading)");

  STASIS_TRACE("reading json from file '{}'", filepath);
  try
  {
    std::ifstream cfg(filepath);
    std::stringstream buffer_ss;
    buffer_ss << cfg.rdbuf();
    this->info = this->info.from(buffer_ss.str());
  } catch (stasis::last_error e)
  {
    STASIS_ERROR("caught an 'stasis::last_error'");
    throw e;
  }

  project_path = fs::path(fs::weakly_canonical(fs::path(this->info.path)) /
                          fs::path(PROJECT_INFO_NAME))
                     .string();

  STASIS_TRACE("project_path = '{}'", project_path);
}

// used when writing
stasis::project::project(project_info data)
{
  STASIS_TRACE("[constructor] ---- stasis::project ---- (writing)");
  project_path = fs::path(fs::weakly_canonical(fs::path(this->info.path)) /
                          fs::path(PROJECT_INFO_NAME))
                     .string();

  if (data.empty())
    ERROR(1, "project info cannot be empty");

  STASIS_TRACE("writing json data to '{}'", project_path);
  try
  {
    std::ofstream cfg(project_path);
    cfg << data.to(data);
  } catch (stasis::last_error e)
  {
    STASIS_ERROR("caught an 'stasis::last_error'");
    throw e;
  }

  this->info = data;
}

// TODO: implement this
stasis::project::project()
{
  STASIS_TRACE("[constructor] ---- stasis::project ---- (empty)");
}

auto stasis::project::empty() -> bool { return this->info.empty(); }

auto stasis::project::exists() -> bool
{
  STASIS_WARNING("(deprecation warning) this function is deprecated and it is "
                 "advised to just get "
                 "the project path some other way and call 'fs::exists' on it");
  return fs::exists(this->project_path);
}

auto stasis::project::operator==(project p) -> bool
{
  return this->info == p.info;
}

auto stasis::project::operator!=(project p) -> bool
{
  return this->info != p.info;
}

auto stasis::project::remove() -> void
{
  STASIS_TRACE("removing file '{}'", this->project_path);
  if (std::remove(this->project_path.c_str()) != 0)
    THROW_ERROR;
}