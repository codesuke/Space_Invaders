# Space Invaders Game

A modern OpenGL-based Space Invaders game written in C++.

## Requirements

- **CMake** 3.10 or higher
- **C++ Compiler** (MSVC, GCC, or Clang)
- **OpenGL** support

### Windows
- Visual Studio 2015 or later, OR
- MinGW with GCC 5.0+

### Linux
- GCC 5.0+ or Clang
- Development libraries for OpenGL and X11

### macOS
- Xcode Command Line Tools

## Dependencies (Included)

- **GLEW 2.1.0** - OpenGL Extension Wrangler
- **GLFW 3.4** - Window and input management
- **GLM 1.0.2** - Mathematics library (header-only)

All dependencies are included in the `utils/` folder, so no external installation is required!

## Building

### Step 1: Clone/Extract the project
```bash
cd path/to/Game
```

### Step 2: Create a build directory
```bash
mkdir build
cd build
```

### Step 3: Generate build files with CMake
**On Windows (Visual Studio):**
```bash
cmake .. -G "Visual Studio 16 2019"
```

**On Windows (MinGW):**
```bash
cmake .. -G "MinGW Makefiles"
```

**On Linux/macOS:**
```bash
cmake ..
```

### Step 4: Build the project
**On Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**On Windows (MinGW) / Linux / macOS:**
```bash
make
```

### Step 5: Run the game
**On Windows:**
```bash
.\bin\space_invaders.exe
```

**On Linux/macOS:**
```bash
./bin/space_invaders
```

## Project Structure

```
Game/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── space_invaders/
│   └── main.cpp           # Game source code
└── utils/
    ├── glew-2.1.0/        # OpenGL Extension Wrangler
    ├── glfw-3.4/          # Window and input library
    └── glm-1.0.2/         # Math library
```

## Quick Start (One Command)

**Windows (PowerShell):**
```powershell
mkdir build; cd build; cmake .. -G "Visual Studio 16 2019"; cmake --build . --config Release; .\bin\space_invaders.exe
```

**Linux/macOS (Bash):**
```bash
mkdir build && cd build && cmake .. && make && ./bin/space_invaders
```

## Troubleshooting

### GLFW Build Fails
- Make sure CMake is in your PATH
- Ensure your compiler is properly installed
- On Linux, you may need to install: `sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev`

### GLEW Library Not Found
- Verify the `utils/glew-2.1.0/lib/Release/` directory exists and contains the appropriate `.lib` or `.a` file
- On Linux/macOS, you may need to rebuild GLEW separately

### OpenGL Not Found
- Windows: Should be included with Visual Studio
- Linux: Install with `sudo apt-get install libgl1-mesa-dev`
- macOS: Should be included with Xcode

## Sharing the Project

Simply share the entire `Game` folder with others. They can follow the **Building** section above to compile and run the game on their system.

**Note:** The build artifacts (generated in the `build/` directory) don't need to be shared. Just the source code and dependencies in `utils/`.

## License

See individual library licenses in their respective directories.
