# Troubleshooting Guide

## Build Issues

### "cmake: command not found"
**Problem:** CMake isn't installed or not in PATH

**Solutions:**
- **Windows:** Download from https://cmake.org/download/ and add to PATH
- **Linux:** `sudo apt-get install cmake`
- **macOS:** `brew install cmake`

Verify: `cmake --version`

---

### "mingw32-make: command not found"
**Problem:** MinGW isn't installed or not in PATH

**Solutions:**
- **Windows:** Download from https://www.mingw-w64.org/
- Add `C:\Program Files\mingw64\bin` to your System PATH
- Restart your terminal

Verify: `gcc --version`

---

### "Cannot find GLEW"
**Problem:** GLEW library files are missing or path is wrong

**Solutions:**
1. Check `utils/glew-2.1.0/lib/Release/` exists
2. Delete `build/` and rebuild from scratch
3. On Windows, ensure `libglew32.a` exists in the lib directory

---

### "GLFW_FOUND = NO"
**Problem:** GLFW CMake couldn't find the library

**Solutions:**
1. Verify `utils/glfw-3.4/CMakeLists.txt` exists
2. Delete `build/` folder
3. Rebuild: `cmake ..` then build again

---

### "error: GLEW_STATIC is not defined"
**Problem:** GLEW compile definitions missing

**Solutions:**
- This is usually auto-handled by CMakeLists.txt
- Try deleting `build/CMakeCache.txt` and rebuilding

---

## Runtime Issues

### Game window won't open
**Problem:** OpenGL context creation failed

**Solutions:**
1. Update your GPU drivers
2. Check OpenGL version: `glxinfo | grep "OpenGL version"` (Linux)
3. Try rebuilding with compatibility mode
4. Ensure you're not in remote desktop (needs GPU acceleration)

---

### Black screen but game runs
**Problem:** Graphics not rendering

**Solutions:**
1. Verify GPU drivers are updated
2. Check that OpenGL 3.3+ is supported
3. Try windowed mode (already default)
4. Check console output for errors

---

### Game crashes on startup
**Problem:** Various possible causes

**Debug:**
```bash
# Check for error messages
./bin/space_invaders 2>&1 | tee output.log
```

Look for errors like:
- `Cannot create OpenGL context`
- `GLEW initialization failed`
- `GLFW window creation failed`

---

## Platform-Specific Issues

### Windows-Specific

**Issue:** "The system cannot find the path specified"
- Solution: Run commands from the correct directory or use full paths

**Issue:** Visual Studio build fails
- Solution: Make sure you're using the correct Visual Studio version in CMake
- Try: `cmake -G "Visual Studio 17 2022" ..`

**Issue:** .exe file is huge (100+ MB)
- Solution: Normal - it's not stripped. Link against MinGW DLLs or use `strip` command

---

### Linux-Specific

**Issue:** "cannot find -lGL"
- Solution: `sudo apt-get install libgl1-mesa-dev`

**Issue:** "cannot find -lX11"
- Solution: `sudo apt-get install libx11-dev`

**Issue:** All X11 errors
- Solution: `sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev`

---

### macOS-Specific

**Issue:** "clang: error: unknown argument"
- Solution: Update Xcode: `xcode-select --install`

**Issue:** GLFW won't link
- Solution: Install Xcode Command Line Tools: `xcode-select --install`

---

## Clean Build (Nuclear Option)

If everything is broken, try a complete clean rebuild:

```bash
# Windows
rmdir /s build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make

# Linux/macOS
rm -rf build
mkdir build
cd build
cmake ..
make
```

---

## Getting Help

If you're still stuck, provide:
1. Your operating system and version
2. Output of `cmake --version`
3. Output of `gcc --version`
4. Full error message from build output
5. Output of `cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON`

---

## Verify Installation

Quick test to see if everything is set up:

```bash
# Check CMake
cmake --version          # Should be 3.10 or higher

# Check compiler
gcc --version            # Windows: mingw version
g++ --version            # Alternative C++ compiler

# Check required files
ls utils/glfw-3.4/CMakeLists.txt
ls utils/glew-2.1.0/include/GL/glew.h
ls space_invaders/main.cpp

# If all these work, you're ready to build!
```

---

## Still Having Issues?

Try these in order:
1. ✅ Check all prerequisites are installed
2. ✅ Verify all project files exist
3. ✅ Run clean build (delete build/ and start over)
4. ✅ Check README.md for OS-specific instructions
5. ✅ Review SETUP.md for detailed setup
6. ✅ Try verbose build: `cmake --build . --verbose`
7. ✅ Post error output on GitHub Issues or forums

Good luck! 🎮
