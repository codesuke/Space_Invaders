# Space Invaders - Cross-Platform Build Guide

## Quick Start (Windows with MinGW)

### Prerequisites
1. **MinGW** - Download from [MinGW](https://www.mingw-w64.org/)
2. **CMake** - Download from [cmake.org](https://cmake.org/download/)
3. **Git** (optional but recommended)

### Build Steps

```bash
# Navigate to project directory
cd path/to/Space_Invaders_Game

# Create and enter build directory
mkdir build
cd build

# Configure with CMake
cmake -G "MinGW Makefiles" ..

# Build the project
mingw32-make

# Run the game
bin/space_invaders.exe
```

---

## Detailed Setup by Operating System

### Windows with MinGW (Recommended for Windows)

**Step 1: Install MinGW**
- Download MinGW64 from https://www.mingw-w64.org/
- Run installer and select:
  - Version: Latest (e.g., 8.1)
  - Architecture: x86_64
  - Threads: posix
  - Exception: seh
  - Build revision: Latest
- Add MinGW to system PATH (usually `C:\Program Files\mingw64\bin`)

**Step 2: Install CMake**
- Download from https://cmake.org/download/
- Choose Windows x64 installer
- Add CMake to system PATH during installation

**Step 3: Clone/Extract Project**
```bash
git clone <repository-url>
cd Space_Invaders_Game
```

**Step 4: Build**
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
./bin/space_invaders.exe
```

---

### Windows with Visual Studio

**Step 1: Install Visual Studio**
- Download Community Edition from https://visualstudio.microsoft.com/
- Install C++ development tools

**Step 2: Install CMake**
- Download from https://cmake.org/download/

**Step 3: Build**
```cmd
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
bin\Release\space_invaders.exe
```

---

### Linux (Ubuntu/Debian)

**Step 1: Install Dependencies**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git
sudo apt-get install -y libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
```

**Step 2: Build**
```bash
mkdir build
cd build
cmake ..
make
./bin/space_invaders
```

---

### macOS

**Step 1: Install Dependencies**
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required tools
brew install cmake git
```

**Step 2: Build**
```bash
mkdir build
cd build
cmake ..
make
./bin/space_invaders
```

---

## Troubleshooting

### "cmake not found"
- **Solution:** Add CMake to your system PATH
- Windows: Control Panel → System → Environment Variables → Add `C:\Program Files\CMake\bin`
- Linux/Mac: `export PATH="/usr/local/bin:$PATH"` in terminal

### "mingw32-make not found"
- **Solution:** Add MinGW to your system PATH
- Verify installation: `gcc --version` in terminal
- Windows: Add `C:\Program Files\mingw64\bin` to PATH

### OpenGL-related errors
- **Windows:** Usually automatic with GPU drivers
- **Linux:** Install: `sudo apt-get install libgl1-mesa-dev`
- **macOS:** Usually automatic; update Xcode if needed

### "Can't find GLEW"
- Make sure you have the complete `utils/` directory with all dependencies
- The project includes: `glew-2.1.0/`, `glfw-3.4/`, `glm-1.0.2/`

### Game runs but window won't open
- Check GPU drivers are up to date
- Ensure OpenGL 3.3+ is supported: `glxinfo | grep "OpenGL version"` (Linux)

---

## Project Structure

```
Space_Invaders/
├── CMakeLists.txt           # Build configuration
├── README.md                # Setup instructions
├── SETUP.md                 # This file
├── space_invaders/
│   └── main.cpp             # Game source code
└── utils/
    ├── glew-2.1.0/          # OpenGL Extension Wrangler
    ├── glfw-3.4/            # Window & Input Management
    └── glm-1.0.2/           # Math Library
```

---

## Game Controls

- **Arrow Keys or A/D:** Move left/right
- **Space:** Shoot
- **P:** Pause/Resume
- **Close Window:** Exit

---

## If Everything Still Doesn't Work

1. **Clean build:** Delete the `build/` directory and start over
2. **Verify files:** Make sure all files in `utils/` are present
3. **Check compiler:** `gcc --version` should show MinGW version
4. **Verbose output:** Run `cmake --build . --verbose` to see detailed error messages

---

## Building from Scratch (All Platforms)

```bash
# Full clean build process
rm -rf build/              # or rmdir /s build on Windows
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

On Windows with MinGW:
```cmd
rmdir /s build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

---

**Questions?** Check that your compiler and CMake versions are recent (CMake 3.10+, GCC 6.0+)
