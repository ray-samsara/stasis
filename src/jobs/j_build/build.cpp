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

#include "build.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <string>
#include <vector>

// local
#include <common/logging.hpp>
#include <output.hpp>
#include <project.hpp>
// :)

static inline auto ends_with(std::string const& value,
                             std::string const& ending) -> bool
{
  if (ending.size() > value.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

auto stasis::build::get_project_files(void) -> std::vector<std::string>
{
  std::vector<std::string> result;
  for (const auto& file :
       std::filesystem::directory_iterator(this->project.path))
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
  for (const auto& file : files)
  {
    if (ends_with(file, extension)) result.push_back(file);
  }
  return result;
}

stasis::build::build(stasis::project_info pi, std::string out_dir)
{
  STASIS_TRACE("INIT! stasis::build");
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
  return 0;
}