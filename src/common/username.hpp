#pragma once

#include <string>

#ifdef _WIN32
#define NOGDI
#define WIN32_LEAN_AND_MEAN
#ifdef __MINGW32__
#include <lmcons.h>
#include <windows.h>
#else
#include <Lmcons.h>
#include <Windows.h>
#endif
static auto get_current_username_win32() -> std::string
{
  wchar_t username[UNLEN + 1];
  DWORD username_len = UNLEN + 1;
  GetUserNameW(username, &username_len);
  std::wstring ws(username);
  return std::string(ws.begin(), ws.end());
}
#else
#include <cstdlib>

static auto get_current_username_unix() -> std::string
{
  return std::string(std::getenv("USER"));
}
#endif

namespace stasis
{
  inline auto get_current_username() -> std::string
  {
#ifdef _WIN32
    return get_current_username_win32();
#else
    return get_current_username_unix();
#endif
  }
} // namespace stasis