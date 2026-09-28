#include <filesystem>

#include "app.hpp"
#include "output.hpp"

#include "jobs/create/create.hpp"

// vendor
#include <spdlog/spdlog.h>

stasis::app::app(std::string argv_0, bool creates, bool builds, std::string dir,
                 std::string name)
{
  spdlog::trace("INIT! stasis::app");

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
  spdlog::trace("stasis::app ~ mode is now {}", this->mode_str_repr);

  if (this->directory.empty())
  {
    spdlog::info("no dir set so dir is now the cwd");
    this->directory = std::filesystem::weakly_canonical(".").string();
  }

  if (mode == stasis::app_mode::create)
  {
    stasis::create_options<stasis::job_create_init_options> i_opts = {
        .options = {stasis::job_create_init_options::use_sensible_defaults}};
    stasis::create job(this->project_name, this->directory, i_opts, {});
    spdlog::trace("about to run that job now...");
    exit_code = job.run();
  }
  else
  {
    stasis::output::warning("mode/job '{}' is not implemented yet",
                            this->mode_str_repr);
    exit_code = 1;
  }

  return exit_code;
}