cmake_minimum_required(VERSION 3.14)

project(
  stasis
  VERSION 0.1.0
  LANGUAGES CXX
  DESCRIPTION "Create and build static webpages without fluff"
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/ProgramOptions.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/ProgramOptions.hxx"
)

set(RANG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/rang"
  "${CMAKE_CURRENT_BINARY_DIR}/rang"
)

add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/inifile-cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/inifile-cpp"
)

add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog"
  "${CMAKE_CURRENT_BINARY_DIR}/spdlog"
)

add_executable(stasis
  src/main.cpp
  src/app.cpp
  src/wrappers/stdio/stdio.cpp
  src/project.cpp
  src/jobs/create/create.cpp
)

target_include_directories(stasis PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(stasis PRIVATE
  ProgramOptionsHxx
  rang
  inicpp::inicpp
  spdlog::spdlog_header_only
)
