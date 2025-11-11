# Project Documentation Overview

## 📚 Main Documentation Files

### For Users Getting Started
- **[README.md](README.md)** - Start here! Overview and quick build instructions
- **[SETUP.md](SETUP.md)** - Detailed setup for Windows, Linux, and macOS
- **[SHARING.md](SHARING.md)** - How to share/distribute the project
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Common issues and solutions

### For Developers
- **[ENHANCEMENTS.md](ENHANCEMENTS.md)** - Suggested features and improvements
- **[CMakeLists.txt](CMakeLists.txt)** - Build configuration
- **[space_invaders/main.cpp](space_invaders/main.cpp)** - Game source code

---

## 🎮 Quick Start

### Windows (MinGW)
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
bin\space_invaders.exe
```

### Linux/macOS
```bash
mkdir build && cd build
cmake ..
make
./bin/space_invaders
```

---

## 📦 Project Structure

```
Game/
├── README.md                 # Quick start guide
├── SETUP.md                  # Detailed OS-specific setup
├── SHARING.md                # How to distribute
├── TROUBLESHOOTING.md        # Common issues & fixes
├── ENHANCEMENTS.md           # Feature suggestions
├── CMakeLists.txt            # Cross-platform build config
│
├── space_invaders/
│   └── main.cpp              # 690 lines of game code
│       ├── Entities: Bullet, Enemy, PowerUp structs
│       ├── GameState: Core game logic
│       ├── Rendering: OpenGL drawing code
│       ├── Input: Keyboard handling
│       └── Main loop: 60 FPS game loop
│
└── utils/                    # Pre-built dependencies
    ├── glfw-3.4/             # Window & input (source)
    ├── glew-2.1.0/           # OpenGL extensions (pre-built)
    └── glm-1.0.2/            # Math library (header-only)
```

---

## 🎮 Game Features

### Core Gameplay
- ✅ Wave-based enemy system (scales with difficulty)
- ✅ Three enemy types: Weak (10pts), Normal (20pts), Tank (50pts)
- ✅ Player movement (Arrows/WASD) and shooting (Space)
- ✅ Collision detection (AABB)
- ✅ Pause functionality (P key)

### Advanced Features
- ✅ **Power-ups:** Shield, Rapid Fire, Multi-shot, Slow Motion
- ✅ **Combo System:** 1x → 1.25x (5 kills) → 1.5x (10 kills) → 2x (20 kills)
- ✅ **High Score System:** Save/load top 10 scores
- ✅ **60 FPS Frame Rate Limiting**
- ✅ **Progressive Difficulty:** Enemies get faster and more aggressive

### Graphics
- Green triangle player ship with shield indicator
- Color-coded enemies (Yellow=Weak, Red=Normal, Dark Red=Tank)
- Colored power-ups with glowing outlines
- Yellow player bullets, Orange enemy bullets

---

## 🛠️ Technologies Used

| Technology | Purpose | Version |
|-----------|---------|---------|
| **C++** | Language | C++17 |
| **OpenGL** | Graphics | 3.3+ (Compatibility Profile) |
| **GLFW** | Window/Input | 3.4 |
| **GLEW** | GL Extensions | 2.1.0 |
| **GLM** | Math | 1.0.2 |
| **CMake** | Build System | 3.10+ |
| **MinGW** | Compiler (Windows) | 6.0+ |

---

## 📋 Build System Information

### Supported Platforms
- ✅ **Windows** (Visual Studio or MinGW)
- ✅ **Linux** (GCC/Clang)
- ✅ **macOS** (Clang)

### Dependencies Handling
- ✅ **Pre-built:** GLEW (compiled libglew32.a for MinGW)
- ✅ **Source:** GLFW (built from source via CMake)
- ✅ **Header-only:** GLM (no compilation needed)

### Build Output
```
build/
├── bin/
│   └── space_invaders.exe    # Main executable
├── lib/
│   ├── libglfw3.a            # GLFW library
│   └── libglew32.a           # GLEW library
└── CMakeFiles/               # Build artifacts
```

---

## 🚀 Distribution

### What to Include
✅ CMakeLists.txt  
✅ README.md, SETUP.md, SHARING.md, TROUBLESHOOTING.md  
✅ space_invaders/ folder  
✅ utils/ folder (with GLFW source, GLEW libs, GLM headers)  

### What NOT to Include
❌ build/ directory  
❌ .exe files  
❌ .o object files  
❌ CMakeCache.txt  

### Sharing Methods
1. **GitHub** - Best for long-term hosting and collaboration
2. **ZIP file** - Simple, portable, easily shareable
3. **Direct copy** - Just copy the folder anywhere

### Recipient Requirements
- CMake 3.10+
- C++ compiler (any version 2015+)
- That's it! GLFW, GLEW, GLM are all included

---

## 🐛 Common Issues & Quick Fixes

| Issue | Cause | Fix |
|-------|-------|-----|
| "cmake not found" | Not installed/not in PATH | Install CMake, add to PATH |
| "mingw32-make not found" | MinGW not installed/not in PATH | Install MinGW, add to PATH |
| "Cannot find GLEW" | Missing lib files | Check utils/glew-2.1.0/lib/Release/ |
| Black screen | OpenGL issue | Update GPU drivers |
| Won't build on Linux | Missing X11 dev libs | `sudo apt-get install libx11-dev` |

**See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for detailed solutions.**

---

## 📖 How to Read This Documentation

**I'm a new user:**
1. Start with [README.md](README.md)
2. Follow [SETUP.md](SETUP.md) for your OS
3. If stuck, check [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

**I want to share this:**
1. Read [SHARING.md](SHARING.md)
2. Tell others to start with [README.md](README.md)

**I want to improve the code:**
1. Read [ENHANCEMENTS.md](ENHANCEMENTS.md) for feature ideas
2. Check [CMakeLists.txt](CMakeLists.txt) for build setup
3. Modify [space_invaders/main.cpp](space_invaders/main.cpp)

**I'm debugging build issues:**
1. Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
2. Try verbose build: `cmake . -DCMAKE_VERBOSE_MAKEFILE=ON`

---

## 📞 Support

If you run into issues:

1. **Check the docs:** README → SETUP → TROUBLESHOOTING
2. **Verify prerequisites:**
   - `cmake --version` (should be 3.10+)
   - `gcc --version` or `g++ --version`
3. **Clean rebuild:**
   ```bash
   rm -rf build/
   mkdir build
   cd build
   cmake ..
   ```
4. **Try verbose output:**
   ```bash
   cmake --build . --verbose
   ```

---

## 🎓 Learning Resources

The game demonstrates:
- **Graphics:** OpenGL 3.3+ fixed-function pipeline
- **Game Loop:** Delta time, 60 FPS limiting
- **Data Structures:** Vectors, arrays for entity management
- **Physics:** AABB collision detection
- **File I/O:** High score persistence
- **Build Systems:** CMake cross-platform configuration

---

**Last Updated:** November 11, 2025  
**Game Status:** ✅ Fully Playable  
**Build Status:** ✅ Cross-Platform Ready
