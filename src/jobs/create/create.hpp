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