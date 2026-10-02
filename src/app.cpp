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

#include "app.hpp"

#include <filesystem>

// local
#include <common/global.hpp>

#include "common/logging.hpp"
#include "jobs/create/create.hpp"
#include "jobs/j_build/build.hpp"
#include "output.hpp"
#include "project.hpp"

namespace fs = std::filesystem;

stasis::app::app(std::string argv_0, bool creates, bool builds, std::string dir,
                 std::string name)
{
  STASIS_TRACE("[constructor] ---- stasis::app ----");
  STASIS_WARNING(
      "this constructor is deprecated; the new constructor shall be used soon");

  execname = argv_0;
  directory = dir;
  if (creates)
    mode = stasis::app_mode::create;
  if (builds)
    mode = stasis::app_mode::build;

  if (creates && builds)
  {
    output::error("only one mode can be set at the same time");
    output::normal("run {} --help for more info", execname);
    std::exit(1);
  }

  mode_str_repr = stasis::app_mode_str_repr.at(mode);
  project_name = name;
}

stasis::app::app(stasis::app_class_constructor a)
{
  STASIS_TRACE("[constructor] ---- stasis::app ---- (newer one)");

  execname = a.argv_0;
  directory = a.dir;
  if (a.creates)
    mode = stasis::app_mode::create;
  if (a.builds)
    mode = stasis::app_mode::build;

  if (a.creates && a.builds)
  {
    output::error("only one mode can be set at the same time");
    output::normal("run {} --help for more info", execname);
    std::exit(1);
  }

  mode_str_repr = stasis::app_mode_str_repr.at(mode);
  project_name = a.name;
  build_output_directory = a.output_dir;
}

auto stasis::app::run(void) -> int
{
  int exit_code;
  /*
  switch (this->mode)
  {
    case app_mode::create:
      {
        // TODO: tweak /.clang-format to fix this ugly formatting
        stasis::create_options<stasis::job_create_init_options> i_opts = {
            .options = {
                stasis::job_create_init_options::use_sensible_defaults}};
        stasis::create job(this->directory, i_opts, {});
        exit_code = job.run();
        break;
      }
    case app_mode::build:
      stasis::output::warning("mode/job '{}' is not implemented yet",
                              this->mode_str_repr);
      exit_code = 1;
      break;
  }
  */

  // std::printf("MODE: %s\n", this->mode_str_repr.c_str());
  STASIS_TRACE("mode is now {}", this->mode_str_repr);

  if (this->directory.empty())
  {
    STASIS_INFO("no dir set so dir is now the cwd");
    this->directory = fs::weakly_canonical(".").string();
  }

  if (mode == stasis::app_mode::create)
  {
    stasis::create_options<stasis::job_create_init_options> i_opts = {
        .options = {stasis::job_create_init_options::use_sensible_defaults}};
    stasis::create job(this->project_name, this->directory, i_opts, {});
    STASIS_TRACE("running job");
    exit_code = job.run();
  }
  else
  {
    stasis::project p(fs::path(this->directory) / fs::path(PROJECT_INFO_NAME));
    stasis::build job(p.info);
    STASIS_TRACE("running job");
    exit_code = job.run();
  }

  return exit_code;
}