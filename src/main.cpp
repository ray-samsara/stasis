#include <iostream>

#include "app.hpp"
#include "output.hpp"
#include "spdlog/common.h"

#include <ProgramOptions.hxx>
#include <rang.hpp>
#include <spdlog/spdlog.h>

auto main(int argc, char *argv[]) -> int
{
#ifdef _WIN32
  rang::setWinTermMode(rang::winTerm::Ansi);
#endif
  rang::setControlMode(rang::control::Force);

  // don't make spdlog print anything at first
  spdlog::set_level(spdlog::level::off);

  po::parser parser;

  auto &flag_verbose =
      parser["verbose"].abbreviation('v').description("Enable debug output");

  auto &flag_build =
      parser["build"].abbreviation('B').description("Build current project");

  auto &flag_create = parser["create"].abbreviation('C').description(
      "Create (or initialize) a project in DIRECTORY");

  std::string name;
  parser["name"].abbreviation('n').description("Name of project").bind(name);

  std::string directory;
  parser[""].description("Target directory (default: .)").bind(directory);

  auto &help =
      parser["help"].abbreviation('h').description("shows this message");

  if (!parser(argc, argv))
  {
    stasis::output::error("couldn't init parser; cannot continue");
    return 1;
  }

  if (argc < 2)
  {
    stasis::output::warning("there is nothing to do");
    stasis::output::normal("run {} --help for more info", argv[0]);
    return 1;
  }

  if (help.was_set())
  {
    std::cout << parser;
    return 0;
  }

  if (flag_verbose.was_set())
    spdlog::set_level(spdlog::level::trace);

  stasis::app app(argv[0], flag_create.available(), flag_build.available(),
                  directory, name);

  spdlog::debug("starting app...");
  return app.run();
}