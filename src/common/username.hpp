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

#include <cstdlib>
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