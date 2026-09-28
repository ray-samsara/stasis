#include <cstdio>
#include <cstring>
#include <filesystem>

#include "project.hpp"

#ifdef _WIN32
#define NOGDI
#endif
#include <common/error.hpp>
#include <wrappers/stdio/stdio.hpp>

// vendor
#include <spdlog/spdlog.h>

// used when reading
stasis::project::project(std::string filepath)
{
  spdlog::trace("INIT! stasis::project (reading constructor)");
  try
  {
    stasis::file cfg(filepath);
    cfg.read();
    this->info = this->info.from(cfg.buffer);
  } catch (stasis::last_error e)
  {
    spdlog::critical("ERROR!!! {}", e.str(true));
    throw e;
  }
  project_path =
      std::filesystem::path(std::filesystem::weakly_canonical(
                                std::filesystem::path(this->info.path)) /
                            std::filesystem::path(".stasis_project.ini"))
          .string();

  // the goal is to check if the target project (or literally, contents of
  // 'project_path' with values same as 'data') exists.
  if (this->exists())
    ERROR(1, "cannot init an already init'ed project");
}

// used when writing
stasis::project::project(project_info data)
{
  spdlog::trace("INIT! stasis::project (writing constructor)");
  project_path =
      std::filesystem::path(std::filesystem::weakly_canonical(
                                std::filesystem::path(this->info.path)) /
                            std::filesystem::path(".stasis_project.ini"))
          .string();

  if (data.empty())
    ERROR(1, "project info cannot be empty");

  try
  {
    stasis::file cfg(".stasis_project.ini");
    cfg.write(data.to(data));
  } catch (stasis::last_error e)
  {
    spdlog::critical("ERROR!!! {}", e.str(true));
    throw e;
  }
  this->info = data;
  // the goal is to check if the target project (or literally, contents of
  // 'project_path' with values same as 'data') exists.
  if (this->exists())
    ERROR(1, "cannot init an already init'ed project");
}

// TODO: implement this
stasis::project::project() { return; }

auto stasis::project::empty() -> bool { return this->info.empty(); }

auto stasis::project::exists() -> bool
{
  // return std::filesystem::exists(this->project_path);

  if (!std::filesystem::exists(this->project_path))
    return false;

  stasis::project_info pi;

  // a never-ending loop will happen if the constructor for reading
  // is just called, SNIP!
  try
  {
    stasis::file cfg(this->project_path);
    cfg.read();
    pi = pi.from(cfg.buffer);
  } catch (stasis::last_error e)
  {
    spdlog::critical("ERROR!!! {}", e.str(true));
    throw e;
  }

  // std::printf("[%s] pi.name: %s ; this->info.name: %s\n", __FUNCTION__,
  //            pi.name.c_str(), this->info.name.c_str());
  spdlog::trace("on function '{}', pi.name = {} and this->info.name = {}",
                __PRETTY_FUNCTION__, pi.name, this->info.name);
  return pi == this->info;
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
  if (std::remove(this->project_path.c_str()) != 0)
    THROW_ERROR;
}