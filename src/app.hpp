#pragma once

#include <map>
#include <string>

namespace stasis
{
  enum class app_mode
  {
    create,
    build
  };
  inline const std::map<app_mode, std::string> app_mode_str_repr = {
      {app_mode::create, "app_mode::create"},
      {app_mode::build, "app_mode::build"}};

  class app
  {
    private:
      std::string execname;
      app_mode mode;
      std::string directory;
      std::string mode_str_repr;
      std::string project_name;

    public:
      app(std::string argv_0, bool creates, bool builds, std::string dir,
          std::string name);
      auto run(void) -> int;
  };
} // namespace stasis