# How to Share This Project with Others

## What to Include

✅ **Always include these:**
- `CMakeLists.txt` - Build configuration
- `README.md` - Overview and quick start
- `SETUP.md` - Detailed setup instructions
- `space_invaders/` folder - All source code
- `utils/` folder - All dependencies (GLFW, GLEW, GLM)
- `.gitignore` - Version control file

❌ **Never include these (they're auto-generated):**
- `build/` directory - Created during build
- `*.exe` files - Compiled binaries
- `*.o` object files - Intermediate build files
- `CMakeCache.txt` - Build cache
- `.vscode/` settings (optional to exclude)

## Sharing Methods

### Option 1: GitHub (Recommended)
```bash
# Initialize git repo (if not already done)
git init
git add .
git commit -m "Initial Space Invaders game commit"

# Add remote and push
git remote add origin https://github.com/yourusername/space-invaders.git
git branch -M main
git push -u origin main
```

Others can clone with:
```bash
git clone https://github.com/yourusername/space-invaders.git
cd space-invaders
```

### Option 2: ZIP File
1. Create a folder named `SpaceInvaders-Game`
2. Copy these into it:
   - CMakeLists.txt
   - README.md
   - SETUP.md
   - space_invaders/ folder
   - utils/ folder
   - .gitignore
3. Right-click → Send to → Compressed (zipped) folder
4. Share the .zip file

### Option 3: Direct File Transfer
Just copy the project folder to another location. The CMake build system will handle everything on the new PC.

---

## What the Person Receiving It Needs

They need to install:
1. **CMake** (3.10+) - https://cmake.org/download/
2. **Compiler:**
   - Windows: Visual Studio OR MinGW
   - Linux: GCC (usually pre-installed)
   - macOS: Xcode Command Line Tools

They DON'T need to install GLEW, GLFW, or GLM - those are included!

---

## Quick Sharing Checklist

```bash
# 1. Clean build artifacts
rm -rf build/        # or: rmdir /s build (Windows)

# 2. Verify essential files exist
ls CMakeLists.txt README.md SETUP.md  # or: dir (Windows)
ls space_invaders/main.cpp
ls utils/glfw-3.4 utils/glew-2.1.0 utils/glm-1.0.2

# 3. Compress
zip -r SpaceInvaders.zip . -x "build/*" "*.exe" "*.o" ".git/*"

# 4. Share!
# Upload to GitHub, Google Drive, etc.
```

---

## File Size Reference

- **With utils/:** ~50-80 MB (includes all dependencies)
- **Without build/:** ~10-15 MB (after .zip compression)
- **Final .zip:** ~5-8 MB (very shareable!)

---

## Testing Before Sharing

Before sending to someone else, verify it works:

```bash
# Clean build
rm -rf build/
mkdir build
cd build

# Try building
cmake ..
mingw32-make  # or: make / cmake --build .

# If it compiles without errors, you're good to share!
```

---

## After Someone Clones/Downloads

They should follow these steps:

```bash
cd SpaceInvaders/
mkdir build
cd build
cmake ..
cmake --build . --config Release  # or mingw32-make / make

# Run the game
./bin/space_invaders    # Linux/macOS
bin\space_invaders.exe  # Windows
```

That's it! They're ready to play! 🎮
