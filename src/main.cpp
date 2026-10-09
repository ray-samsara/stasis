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

#include <iostream>

// local
#include <common/logging.hpp>

#include "app.hpp"
#include "output.hpp"

// vendor
#include <ProgramOptions.hxx>
#include <rang.hpp>

auto main(int argc, char *argv[]) -> int
{
#ifdef _WIN32
  rang::setWinTermMode(rang::winTerm::Ansi);
#endif
  rang::setControlMode(rang::control::Force);

  // don't log anything at first unless it's fatal
  stasis::logging::current_level = stasis::logging::level::NoLog;

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

  std::string output;
  parser["output"]
      .abbreviation('o')
      .description("('--build' only) Directory to write output on")
      .bind(output);

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
    stasis::logging::current_level = stasis::logging::level::Trace;

  // stasis::app app(argv[0], flag_create.available(), flag_build.available(),
  //                directory, name);
  stasis::app_class_constructor app_args = {
      .creates = flag_create.available(),
      .builds = flag_build.available(),
      .dir = directory,
      .name = argv[0],
      .output_dir = output,
  };

  stasis::app app(app_args);

  STASIS_TRACE("starting app");
  return app.run();
}