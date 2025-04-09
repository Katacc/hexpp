<div align="center">
    <h1>proj. Hex++</h1>
    C++ project manager. Wrapper to combine CMake, vcpkg, and project generation.

   > Made to ease your C++ project management with a bit of hex magix ~

</div>

https://github.com/Katacc/hexpp (main)
https://gitlab.com/Katacc/hexpp

---

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Y8Y51D6K2E)

## Release and plans

Hello! Im proud to release my take on C++ project management for the public!

The project is in working state and has the core functionalities I wanted it to have. There shouldn't be any big changes that would break projects, but I can't say for sure yet. Ill let you know if any updates comes that might break functionality, and how to migrate

TODO:
   - [ ] Argument passing for the target runnable
   - [ ] "self-heal" command
   - [ ] Regex matching for configurations to everywhere, currently configs are strict on spacing

## Getting Started

### Prerequisites

This project uses CMake, GCC, and G++ compilers, which can be installed natively on Linux and macOS or through MSYS2 for Windows.

#### Required Tools:
- `g++` or `clang` (c++23 support)
- `gcc` or `clang`
- `gdb`
- `CMake` (4.0 or later recommended)
- `Make`
- `vcpkg` (installation guide below)
- `ninja`

#### Windows-Specific Requirements:
If you are on Windows and want to use the default vcpkg compiler settings, you need to have **MSVC (Microsoft Visual C++ Compiler)** installed.

---

### Installing vcpkg

vcpkg is a C/C++ package manager made by Microsoft: [https://vcpkg.io](https://vcpkg.io). You can follow the guide on their website or use the steps below.

#### Steps to Install vcpkg:
1. Clone the repository:
   ```sh
   git clone https://github.com/microsoft/vcpkg.git
   ```

2. Run the bootstrap script:
   - **Linux/macOS**:
     ```sh
     cd vcpkg && ./bootstrap-vcpkg.sh
     ```
   - **Windows**:
     ```sh
     cd vcpkg
     .\bootstrap-vcpkg.bat
     ```

3. Configure the `VCPKG_ROOT` environment variable:
   Add your vcpkg root directory to your PATH. For example:
   ```sh
   export VCPKG_ROOT=/path/to/vcpkg
   export PATH=$VCPKG_ROOT:$PATH
   ```
   > **Note**: You need to do this every time you launch a new terminal. Consider adding it permanently to your PATH and creating a new environment variable `VCPKG_ROOT`.

---

### Building Hex++

1. Clone the repository:
   ```sh
   git clone https://github.com/Katacc/hexpp.git
   cd hexpp
   ```

2. Use CMake to build the project (gcc):
   - **Default Preset (gcc)**:
   ```sh
   cmake --preset=default
   cmake --build build --config Release
   ```

3. clang users:
   - **clang Preset**:
   ```sh
   cmake --preset=clang
   cmake --build build --config Release
   ```


4. Locate the executable:
   The executable will be located in the [`build/Release`](build/Release/ ) folder. You can copy it to your PATH to use it anywhere.

   - **Linux Example**:
   ```sh
   chmod +x ./build/Release/hx++
   sudo cp ./build/Release/hx++ /usr/bin/
   ```

   - **Windows Example**:
   Copy the `build/Release/hx++.exe` to a folder (e.g., `bin`) and add that folder to your PATH.


#### Find writing hx++ annoying?

After cloning hexpp repository, go into `project.cfg`, and change the project name to hxpp, this makes the executable into hxpp(.exe)
```
[project]
name = hxpp
version = 1.0.0

[CMake]
CXX_version = 23
```

if you have built the project allready with the original name, don't worry, just delete `/build` folder and build again after changing the value in project.cfg

---

### Using Hex++

Hex++ is a C++ project manager that creates and initializes a C++ project for you, it can build, run and add packages.

#### Project Structure:
When you create a new project, it will have the following structure:
```sh
project
│
├── build/                  # Build files (generated by CMake)
│
├── .vscode/                # VSCode configuration files
│   ├── tasks.json          # Build tasks
│   ├── launch.json         # Debug configurations
│
├── src/                    # Source code
│   ├── main.cpp            # Main entry point of the application
│   ├── libraries/          # Header-only libraries
│   │   └── test.h          # Example library file
│
├── project.cfg             # Stores the project name and version
├── CMakeLists.txt          # CMake configuration file
├── CMakePresets.json       # CMake presets for build configurations
├── CMakeUserPresets.json   # User-specific CMake presets
├── .gitignore              # Git ignore file
└── vcpkg.json              # vcpkg package configuration
```

#### Commands:
1. **Create a New Project**:
   ```sh
   hx++ new <project_name>
   cd <project_name>
   ```

2. **Build the Project**:
   ```sh
   hx++ build [config] [preset]
   ```
   Example:
   ```sh
   hx++ build     # Builds the project with Debug and default preset
   ```

3. **Run the Project Executable**:
   ```sh
   hx++ run [config]
   ```
   You can also pass arguments to the target program:
   ```sh
   hx++ run [config] <arguments>
   ```

4. **Add a Package Through vcpkg**:
   ```sh
   hx++ add <package>
   ```
   Example:
   ```sh
   hx++ add sdl3
   ```

   **Install packages when you have not added them**:
   ```sh
   hx++ install
   ```

   After adding a package, you need to include it in your [`CMakeLists.txt`](CMakeLists.txt ) file. For example:
   ```cmake
   find_package(SDL3 CONFIG REQUIRED)
   target_link_libraries([PROGRAM_NAME] PRIVATE SDL3::SDL3)
   ```

---

### Configuring

If configurations are not set, file not found or other reasons, the program defaults to its set default values, to use `gcc` and `g++` and default profile for building.

Hex++ can be configured with a configuration file. To do so, create new folder `hex++` in your `~/.config` and add a file
`config.ini` there
> ~ = User home folder

`~/.config/hex++/config.ini`


Following configurations are available, the first three configs impacts the `custom` build preset and the last configuration value sets your default preset the build command uses. The configurations does not affect default, vcpkg or clang profiles!

eg: `default_preset = clang` This uses clang preset by default on build, if you don't specify parameters
```
CXX_COMPILER_PATH = clang++
C_COMPILER_PATH = clang
C_DEBUGGER_PATH = gdb

default_preset = default

```
Note, pay attention to the syntax (correct spacing)\
Note2, add an empty line after configurations just in case!

---

### Changing C++ version

Change the variable in `project.cfg` \
`CXX_version` default will be 23

changing CMake version will be done the CMake way in `CMakeLists.txt`

---

### Adding Files to the Project

#### Header-Only Files:
To add header-only files (`.h`), place them in the `libraries` folder and include them in your source files:
```cpp
#include "test.h"
```

#### Header + Implementation Files:
CMake is configured to find all .cpp files and automatically added to the project, if using recent enough CMake version. (4.0 recommended)

include header files in your source files normally. Use absolute paths from src
```cpp
// In main.cpp
#include "commands/headers/test.h"
```

#### Modules

Adding modules is simple, CMake automatically grabs all module files (.ixx) and includes them for you, you only need to do the standard C++23 module exports and imports in your program.

---



### Changing the Project Name and version

To change the project name:
1. Update the `project.cfg` file with the new name.
2. Update the `project.cfg` file with the new version
```
[project]
name = project_name
version = 1.0
```

---

### Encountering Errors

#### Common Issues:
1. **Build Errors**:
   - Delete the [`build`](build ) folder and try again:
     ```sh
     rm -rf build/
     hx++ build
     ```

2. **Switching Between presets**:
   - You **must** delete the [`build`](build ) folder before switching:
     ```sh
     rm -rf build/
     ```

3. **Missing Dependencies**:
   - Ensure all required tools (e.g., `gcc`, `g++`, `ninja`) are installed and available in your PATH.

#### Reporting Issues:
If you encounter unexpected errors, please submit an issue describing the problem.

Preferably use GitHub for any issues / pull requests https://github.com/Katacc/hexpp

---

### Example Workflow

1. Create a new project:
   ```sh
   hx++ new my_project
   cd my_project
   ```

2. Add a package:
   ```sh
   hx++ add sdl3
   ```

3. Build the project:
   ```sh
   hx++ build
   ```

4. Run the project:
   ```sh
   hx++ run
   ```

---

### project.cfg syntax

project.cfg syntax is strict, since many things read from it, make sure the that there is 1 space in between values and = marks.
(No regex matching for now)

```
[project]
name = hx++
version = 1.0.0

[CMake]
CXX_version = 23
```

---

### Specific package problems
if you find a package thats problematic (and even maybe a fix) open issue in GitLab

- fmt
   - problem: refuses to compile
   - fix: Use fmt-header-only

- sdl3-image
   - problem: No png support
   - fix: install it using png flag `sdl3-image[png]`


---

Enjoy using Hex++!