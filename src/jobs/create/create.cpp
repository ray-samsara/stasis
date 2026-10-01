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

#include <cerrno>
#include <filesystem>

// local
#include <common/error.hpp>
#include <common/logging.hpp>
#include <common/username.hpp>
#include <output.hpp>
#include <project.hpp>

#include "create.hpp"
// :)

namespace fs = std::filesystem;

#define FAIL(e)                                                                \
  {                                                                            \
    stasis::output::error("project init failed: {}", e.errmsg);                \
    return 1;                                                                  \
  }

stasis::create::create(std::string name, std::string dir,
                       create_options<job_create_init_options> i_opts,
                       create_options<job_create_meta_options> m_opts)
{
  stasis::logging::current_tail = "create";
  STASIS_TRACE("[constructor] ---- stasis::create ----");
  project_name = name;
  directory = fs::weakly_canonical(dir);
  init_options = i_opts;
  meta_options = m_opts;
}

auto stasis::create::run(void) -> int
{
  STASIS_TRACE("---- begin job stasis::create::run() ----");

  stasis::project project;
  std::string project_path =
      fs::weakly_canonical(fs::path(this->directory)).string();
  std::string project_file =
      fs::path(project_path) / fs::path(".stasis_project.ini");

  if (this->project_name.empty())
  {
    STASIS_INFO("no project name set so using '.stem()' of this->directory "
                "({}) as project name",
                this->directory);
    project_name =
        fs::weakly_canonical(fs::path(this->directory)).stem().string();
  }

  stasis::output::normal("creating project '{}' in '{}'", project_name,
                         this->directory);

  // interactive mode
  if (init_options.has(job_create_init_options::interactive_mode))
  {
    stasis::output::warning("interactive mode is not implemented yet");
    return 1;
  }

  // we'd want to see if the project to be created already exists before doing
  // anything else
  STASIS_INFO("checking if a project like this exists");
  STASIS_TRACE("checking path: '{}'", project_file);
  if (fs::exists(project_file))
  {
    STASIS_ERROR("a project already exists");
    stasis::output::error(
        "cannot init project; directory '{}' already has a project file",
        project_path);
    return 1;
  }

  STASIS_WARNING("file '{}' deos not exist; proceeding", project_file);

  // sensible defaults
  if (init_options.has(job_create_init_options::use_sensible_defaults))
  {
    STASIS_TRACE("using sensibile defaults for project creation");
    stasis::project_info default_project_info = {
        .name = project_name,
        .path = project_path,
        .version = "0.1.0",
        .author = stasis::get_current_username(),
        .license = "MIT",
    };
    STASIS_TRACE("default_project_info.path = '{}'", default_project_info.path);

    try
    {
      STASIS_TRACE(
          "passing 'default_project_info' as data to actual project class");
      project = stasis::project(default_project_info);
    } catch (stasis::last_error e)
    {
      STASIS_TRACE("e: {}", e.str(true));
      stasis::output::error("project '{}' init failed: '{}'",
                            default_project_info.name, e.errmsg);
      return 1;
    }
  }

  if (project.exists())
  {
    stasis::output::normal("created project '{}' on path '{}'",
                           project.info.name, project.info.path);
    return 0;
  }

  return 1;
}