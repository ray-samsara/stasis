#include <filesystem>

#include "create.hpp"

#include <common/error.hpp>
#include <common/username.hpp>
#include <output.hpp>
#include <project.hpp>

// vendor
#include <spdlog/spdlog.h>

stasis::create::create(std::string name, std::string dir,
                       create_options<job_create_init_options> i_opts,
                       create_options<job_create_meta_options> m_opts)
{
  spdlog::trace("INIT! stasis::create");
  project_name = name;
  directory = dir;
  init_options = i_opts;
  meta_options = m_opts;
}

auto stasis::create::run(void) -> int
{
  stasis::project project;
  if (this->project_name.empty())
  {
    spdlog::info("no project name set so using '.stem()' of this->directory "
                 "({}) as project name",
                 this->directory);
    project_name = std::filesystem::weakly_canonical(
                       std::filesystem::path(this->directory))
                       .stem()
                       .string();
  }

  // interactive mode
  if (init_options.has(job_create_init_options::interactive_mode))
  {
    stasis::output::warning("interactive mode is not implemented yet");
    return 1;
  }

  stasis::output::normal("creating project '{}' in '{}'", project_name,
                         this->directory);

  // sensible defaults
  if (init_options.has(job_create_init_options::use_sensible_defaults))
  {
    spdlog::trace("using sensibile defaults for uhh, creating projects");
    stasis::project_info default_project_info = {
        .name = project_name,
        .path = std::filesystem::weakly_canonical(
                    std::filesystem::path(this->directory))
                    .string(),
        .version = "0.1.0",
        .author = stasis::get_current_username(),
        .license = "MIT",
    };

    try
    {
      project = stasis::project(default_project_info);
    } catch (stasis::last_error e)
    {
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