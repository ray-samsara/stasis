## Building

### TL;DR
```sh
git clone https://github.com/ray-samsara/stasis.git
cd stasis
git submodule init
chmod +x ./build.sh
./build.sh

# Uncomment below line (NOT THIS!!) to run stasis immediately
# ./stasis --help
```

**Contents of ./build.sh**
```sh
#!/usr/bin/env bash

set -xe

cmake -B ./build/stasis -S . -G "Unix Makefiles"
cmake --build ./build/stasis -j "$(nproc)"
```


>[!WARNING]
>Before proceeding, make sure you have followed [Prerequisites](#prerequisites) before building stasis.

### Prerequisites
Paste and run this command into your terminal in the root of this project.
```sh
git submodule init
```
This pulls in all the dependencies needed to build this project. (don't worry, there aren't many)

### Dependencies
stasis, like other projects of mine, **vendors** dependencies. This means nothing has to be installed from any kind of package managers.

<!-- TODO: href these -->
stasis depends on...
1. **nlohmann/json**: for parsing project info
2. **ProgramOptions.hxx**: for the CLI
3. **rang**: for colors in the terminal
4. **maddy**: markdown parsing

>[!WARNING]
>Terminal colors may not show on Microsoft Windows 10 1507 and older.