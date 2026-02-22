# Test Project

A cross-platform C++ project using CMake, CMakePresets, Ninja, and vcpkg.

## Prerequisites

- **CMake** 3.21 or higher
- **Ninja** build system
- **vcpkg** package manager (optional but recommended)
- **C++23** compatible compiler (GCC, Clang, or MSVC)
- **VS Code** with recommended extensions

### Installing Required Tools (Linux/Ubuntu)

```bash
# Install build essentials (includes g++ compiler)
sudo apt update
sudo apt install build-essential cmake ninja-build git

# Verify installations
cmake --version
ninja --version
g++ --version
```

### Setting up vcpkg (Optional)

1. Clone vcpkg:
```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
```

2. Bootstrap vcpkg:
   - Linux/macOS: `./bootstrap-vcpkg.sh`
   - Windows: `.\bootstrap-vcpkg.bat`

3. Set the `VCPKG_ROOT` environment variable:
```bash
sudo nano ~/.bashrc
export VCPKG_ROOT=/path/to/vcpkg
```

## Building the Project

### Quick Start

First, verify all dependencies are installed:
```bash
./check-deps.sh
```

If you see missing dependencies, install them:
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git
```

**Note:** If you get CMake errors about toolchain files, clean the build directory:
```bash
rm -rf build
```

### Using CMake Presets (Example in Linux)

#### Configure
```bash
# For Debug build
cmake --preset linux-debug

# For Release build
cmake --preset linux-release
```

#### Build
```bash
# Debug
cmake --build --preset linux-debug

# Release
cmake --build --preset linux-release
```

### Using VS Code

1. Open the project folder in VS Code
2. Install recommended extensions when prompted
3. Use the CMake Tools extension to select a preset
4. Build using:
   - `Ctrl+Shift+B` (default build task)
   - Or select "CMake: Build" from the Command Palette

### Debug in VS Code

1. Set breakpoints in your code
2. Press `F5` or select "C++ Debug (Linux)" from the Run and Debug panel
3. The project will automatically build and launch the debugger

## Running the Application

```bash
# Debug build
./build/linux-debug/TestProject

# Release build
./build/linux-release/TestProject
```

## Cross-Platform Support

The project includes presets for:
- **Linux**: `linux-debug`, `linux-release`
- **Windows**: `windows-debug`, `windows-release`
- **macOS**: `macos-debug`, `macos-release`

The appropriate preset is automatically selected based on your host system.

## Adding Dependencies

To add a vcpkg package, edit `vcpkg.json`:

```json
{
  "dependencies": [
    "fmt",
    "nlohmann-json"
  ]
}
```

Then reconfigure the project to install the dependencies.

## Troubleshooting

### Build errors when pressing Ctrl+Shift+B

1. **"CMake was unable to find a build program corresponding to Ninja"**
   - Install Ninja: `sudo apt install ninja-build`

2. **"CMAKE_CXX_COMPILER not set"**
   - Install build tools: `sudo apt install build-essential`

3. **"Could not find toolchain file: /scripts/buildsystems/vcpkg.cmake"**
   - Clean the build directory: `rm -rf build`
   - Reconfigure: `cmake --preset linux-debug`
   - Or set VCPKG_ROOT if you want to use vcpkg

### Verifying Dependencies

Run the dependency checker script:
```bash
./check-deps.sh
```

This will show you what's installed and what's missing.
