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

#include <algorithm>
#include <exception>
#include <filesystem>
#include <format>
#include <sstream>
#include <string>
#include <vector>

// local
#include "build.hpp"

#include <common/error.hpp>
#include <common/logging.hpp>
#include <output.hpp>
#include <project.hpp>
#include <wrappers/stdio/stdio.hpp>
// :)

// vendor
#include <maddy/parser.h>

namespace fs = std::filesystem;

static inline auto ends_with(std::string const &value,
                             std::string const &ending) -> bool
{
  if (ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

auto stasis::build::get_project_files(void) -> std::vector<std::string>
{
  std::vector<std::string> result;
  for (const auto &file : fs::directory_iterator(this->project.path))
  {
    result.push_back(file.path().string());
  }
  return result;
}

auto stasis::build::filter_files_by_extension(std::vector<std::string> files,
                                              std::string extension)
    -> std::vector<std::string>
{
  std::vector<std::string> result;
  for (const auto &file : files)
  {
    if (ends_with(file, extension))
      result.push_back(file);
  }
  return result;
}

auto stasis::build::parse_file_to_directory(std::string file, fs::path dirpath)
    -> void
{
  std::stringstream md_src_stream;
  std::string md_src;
  try
  {
    stasis::file md_file(file);
    md_file.read();
    md_src = md_file.buffer;
    assert(!md_src.empty());
  } catch (stasis::last_error e)
  {
    THROW_ERROR;
  }
  md_src_stream << md_src;

  maddy::Parser parser;
  STASIS_TRACE("---- output of '{}' -----", file);
  STASIS_TRACE("{}", parser.Parse(md_src_stream));
  STASIS_TRACE("----   end output   -----");
}

stasis::build::build(stasis::project_info pi, std::string out_dir)
{
  STASIS_TRACE("[constructor] ---- stasis::build ----");
  project = pi;
  if (project.empty())
  {
    STASIS_ERROR(
        "project info cannot be empty; informing this to the end user");
    ERROR(1, "project info cannot be empty");
  }
  output_directory = out_dir;
  if (output_directory.empty())
  {
    output_directory = std::format("stasis-build-{}", project.name);
  }
}

auto stasis::build::run(void) -> int
{
  stasis::output::normal("starting build for project '{}'", this->project.name);
  stasis::output::normal("output will be written to '{}'",
                         this->output_directory);
  stasis::output::normal("finding markdown files to compile");
  std::vector<std::string> markdown_files =
      this->filter_files_by_extension(this->get_project_files(), ".md");
  stasis::output::normal("found {} files", markdown_files.size());
  if (markdown_files.size() == 0)
  {
    stasis::output::error("no markdown files found; nothing to build");
    return 1;
  }

  STASIS_INFO("[step 1] creating directories");
  stasis::output::normal("creating build directory");

  STASIS_TRACE("[step 1] checking if '{}' exists and if it's a file",
               this->output_directory);
  if (fs::exists(this->output_directory) &&
      !fs::is_directory(this->output_directory))
  {
    STASIS_ERROR("'{}' is a file; cannot continue", this->output_directory);
    stasis::output::error("'{}' already exists and it is not a directory");
    return 1;
  }

  fs::path dirpath = fs::weakly_canonical(fs::path(this->output_directory));
  STASIS_TRACE("[step 1] creating directory in '{}'", dirpath.string());
  try
  {
    fs::create_directory(dirpath);
  } catch (const std::exception &e)
  {
    STASIS_ERROR("[step 1] caught exception: {}", e.what());
    stasis::output::error("couldn't create build directory in '{}': {}",
                          dirpath.string(), e.what());
    return 1;
  }

  STASIS_INFO("[step 2] parsing markdown files");
  stasis::output::normal("compiling project...");

  try
  {
    this->parse_file_to_directory(markdown_files[0], fs::path("."));
  } catch (stasis::last_error e)
  {
    STASIS_ERROR("caught exception:");
    STASIS_ERROR("{}", e.str(true));
    STASIS_TRACE("reporting back to user");
    stasis::output::error("cannot parse markdown file '{}': {}",
                          markdown_files[0], e.errmsg);
    return 1;
  }

  return 0;
}