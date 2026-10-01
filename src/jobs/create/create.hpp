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

#pragma once

#include <string>
#include <vector>

namespace stasis
{
  enum class job_create_init_options
  {
    interactive_mode,
    use_sensible_defaults,
  };

  enum class job_create_meta_options
  {
    dont_init_git_repo,
    empty_project,
  };

  // TODO: make use of this
  template <typename O> struct create_options
  {
      std::vector<O> options;

      auto has(O option) const -> bool
      {
        for (auto &opt : options)
        {
          return option == opt;
        }
      }
  };

  class create
  {
    private:
      std::string directory, project_name;
      create_options<job_create_init_options> init_options;
      create_options<job_create_meta_options> meta_options;

    public:
      create(std::string name, std::string dir,
             create_options<job_create_init_options> i_opts,
             create_options<job_create_meta_options> m_opts);

      auto run(void) -> int;
  };
} // namespace stasis