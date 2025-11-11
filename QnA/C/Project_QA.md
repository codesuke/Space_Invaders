# Space Invaders Project - Q&A

## 🏗️ Project Setup

### Q: How was the project initially setup?
**A:** The project uses CMake as the build system with three pre-included libraries:
1. **GLFW 3.4** - Compiled from source during build
2. **GLEW 2.1.0** - Pre-compiled static libraries included
3. **GLM 1.0.2** - Header-only math library

The build generates a `CMakeLists.txt` that automatically detects your compiler (MinGW, MSVC, GCC, or Clang) and configures everything correctly.

---

### Q: What's the project directory structure?
**A:** 
```
Game/
├── space_invaders/
│   └── main.cpp           (690 lines, complete game)
├── CMakeLists.txt         (Build configuration)
├── utils/
│   ├── glfw-3.4/          (Window/input library source)
│   ├── glew-2.1.0/        (OpenGL extensions library)
│   └── glm-1.0.2/         (Math library headers)
├── guides/                (Documentation)
├── build/                 (Compiled output)
└── highscores.txt         (Game persistence)
```

---

### Q: How do I build the project?
**A:** 
1. **Windows (MinGW):**
   ```bash
   mkdir build && cd build
   cmake -G "MinGW Makefiles" ..
   mingw32-make
   ```

2. **Linux/macOS (GCC/Clang):**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run:** `./build/bin/space_invaders` (Linux/macOS) or `build\bin\space_invaders.exe` (Windows)

---

## 📚 Libraries & Tools

### Q: Why use these specific libraries?

| Library | Purpose | Why Chosen |
|---------|---------|-----------|
| **GLFW** | Window & Input | Industry standard, cross-platform, handles OS-specific windowing |
| **GLEW** | OpenGL Loading | Manages OpenGL extension pointers, simplifies GL calls |
| **GLM** | Math/Vectors | Header-only, optimized, standard for graphics programming |
| **CMake** | Build System | Cross-platform, works with any compiler, auto-detects setup |
| **C++ STL** | Data Structures | `std::vector` (bullets/enemies), `std::array` (scores) |

---

### Q: What does each library do?

#### GLFW (Graphics Library Framework)
- **What:** Creates and manages the game window
- **Functions Used:**
  - `glfwCreateWindow()` - Opens the game window
  - `glfwPollEvents()` - Gets keyboard/mouse input
  - `glfwSwapBuffers()` - Displays rendered frame
  - `glfwGetTime()` - Gets delta time for frame-rate limiting
- **Why:** Handles all the messy OS stuff (Windows/Linux/macOS windowing)

#### GLEW (OpenGL Extension Wrangler)
- **What:** Loads all modern OpenGL functions
- **Functions Used:**
  - `glewInit()` - Initializes all GL function pointers
  - `glGenBuffers()`, `glBindBuffer()`, `glBufferData()` - GPU memory management
  - `glDrawArrays()` - Renders geometry
- **Why:** Modern OpenGL requires dynamic function loading; GLEW automates this

#### GLM (OpenGL Mathematics)
- **What:** Provides vector and matrix types (`glm::vec3`, `glm::mat4`)
- **Functions Used:**
  - `glm::perspective()` - Camera projection setup
  - `glm::translate()`, `glm::scale()` - Object transformations
- **Why:** GPU shaders use vectors/matrices; GLM makes math code clean and fast

#### C++ STL (Standard Template Library)
- **What:** Built-in data structures
- **Used:**
  - `std::vector<Bullet> bullets;` - Dynamic arrays for game entities
  - `std::array<HighScore, 10>` - Fixed-size score storage
  - `std::algorithm` - Sorting scores
- **Why:** Battle-tested, optimized, no external dependency

---

## 🛠️ How Tools Work Together

### Build Pipeline:
```
source code (main.cpp)
    ↓
CMake (reads CMakeLists.txt, generates compiler instructions)
    ↓
Compiler (gcc/clang/msvc - compiles C++ to machine code)
    ↓
Linker (links game code + GLFW + GLEW libraries)
    ↓
Executable (space_invaders.exe)
```

### Runtime Flow:
```
main()
  ├─ GLFW: Create window
  ├─ GLEW: Load OpenGL functions
  ├─ Game Loop:
  │   ├─ GLFW: Poll input
  │   ├─ Update game logic
  │   ├─ OpenGL: Render to GPU
  │   └─ GLFW: Swap display buffers
  └─ GLFW: Cleanup
```

---

## 💭 Monolithic vs Modular Design

### Q: Why is the game in ONE file (monolithic)?

**A:** The current design uses a single `main.cpp` (690 lines) because:

| Reason | Benefit |
|--------|---------|
| **Single Responsibility** | Everything is in one place = easier to understand flow |
| **No Header Complexity** | Avoids circular dependency issues common in header-only designs |
| **Fast Compilation** | One file compiles instantly; modular can have 10+ files to recompile |
| **Clear Dependencies** | All `#include` statements at the top; easy to see what's used |
| **Easy Sharing** | Copy one file to another PC = project works (dependencies still needed) |
| **Beginner-Friendly** | New developers read one file top-to-bottom, not jump between headers |

---

### Q: What are the code sections in main.cpp?

**A:** The file is organized in this order:

1. **Includes** (lines 1-12) - All external dependencies
2. **Macros** (line 14) - `GL_ERROR_CASE` for debugging
3. **Debug Function** (lines 16-38) - `gl_debug()` for OpenGL errors
4. **High Score Functions** (lines 40-90) - File I/O for persistence
5. **Data Structures** (lines 92-130) - `Bullet`, `PowerUp`, `Enemy`, `HighScore`
6. **GameState Class** (lines 132-600) - Core game logic
   - Constructor & initialization
   - Input handling
   - Update logic (collision, spawning)
   - Rendering
   - Helper functions (spawning, collision detection)
7. **Shader Code** (lines 602-650) - GLSL vertex/fragment shaders as strings
8. **Main Function** (lines 652-690) - Entry point, game loop

---

### Q: Can you make it modular?

**A:** Technically yes, but here's the trade-off:

| Aspect | Monolithic | Modular |
|--------|-----------|---------|
| **Files** | 1 (main.cpp) | 6-8 (headers + cpp) |
| **Compilation** | ~2 seconds | ~5-10 seconds |
| **Learning Curve** | Easy (read 1 file) | Medium (follow includes) |
| **Maintenance** | All in one place | Spread across files |
| **Sharing** | Still need utils/ folder | Still need utils/ folder |
| **Code Reuse** | Hard (copy sections) | Easy (include header) |

**Modular Structure Could Be:**
```
space_invaders/
├── main.cpp           (50 lines - just game loop)
├── entities.h         (structures: Bullet, Enemy, PowerUp)
├── game.h/.cpp        (GameState class)
├── rendering.h        (draw functions)
├── input.h            (input handling)
└── highscores.h       (score management)
```

**Recommendation:** Keep monolithic for now. Once project grows to 2000+ lines, refactor then.

---

## 🎮 Thought Process & Design

### Q: Why this game architecture?

**A:** The `GameState` class encapsulates everything:

```cpp
class GameState {
    // Game data (state)
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    glm::vec2 playerPos;
    
    // Game behavior (methods)
    void update(float dt);
    void handleInput(GLFWwindow* window);
    void render();
};
```

**Why this design?**
- **OOP Encapsulation** - Related data + functions grouped together
- **Single Game Object** - Can have multiple `GameState` for menu, pause, etc.
- **Easy Serialization** - Save/load game state easily
- **Testability** - Unit test the GameState independently

---

### Q: What's the game loop?

**A:** Standard game loop pattern:

```cpp
while (!glfwWindowShouldClose(window)) {
    // 1. Calculate delta time
    float dt = getDeltaTime();
    
    // 2. Input
    gameState.handleInput(window);
    
    // 3. Update
    gameState.update(dt);
    
    // 4. Render
    glClear(GL_COLOR_BUFFER_BIT);
    gameState.render();
    glfwSwapBuffers(window);
    
    // 5. Frame rate limiting (60 FPS)
    limitFrameRate(60);
}
```

**Flow:** Input → Update Logic → Render → Display → Repeat at 60 FPS

---

### Q: How is collision detection done?

**A:** Axis-Aligned Bounding Box (AABB) collision:

```cpp
struct AABB {
    float left, right, top, bottom;
};

bool collides(AABB a, AABB b) {
    return !(a.right < b.left || a.left > b.right ||
             a.bottom < b.top || a.top > b.bottom);
}
```

**Why AABB?**
- Fast: 4 comparisons per collision check
- Simple: Just rectangles, no complex math
- Good enough: Works well for 2D retro games

**Alternative:** Circle-circle collision (used for power-ups)

---

### Q: Why frame-rate limiting at 60 FPS?

**A:** 
- **60 FPS** = 16.67ms per frame
- Achievable on most hardware
- Common monitor refresh rate (60Hz)
- Smooth enough for human eyes
- Doesn't waste battery on high-end hardware

**Implementation:** 
```cpp
double targetFrameTime = 1.0 / 60.0;  // ~0.0167 seconds
while (true) {
    double frameStart = glfwGetTime();
    // ... game logic ...
    double frameTime = glfwGetTime() - frameStart;
    if (frameTime < targetFrameTime) {
        sleep(targetFrameTime - frameTime);  // Wait
    }
}
```

---

## 📦 Basic Functions & Core Systems

### Q: What are the main game functions?

**A:**

| Function | Purpose | Called |
|----------|---------|--------|
| `update(float dt)` | Physics, collision, spawning | Every frame |
| `handleInput(GLFWwindow*)` | Keyboard/mouse input | Every frame |
| `render()` | Draw all objects | Every frame |
| `spawnWave()` | Generate enemy wave | When wave completes |
| `spawnPowerUp(x, y)` | Add power-up | Random on kill |
| `spawnBullet(x, y)` | Add bullet | On shoot |
| `checkCollisions()` | Detect all collisions | In update() |

---

### Q: How does wave difficulty work?

**A:** 
```cpp
void spawnWave() {
    int enemyCount = 3 + wave;           // More enemies per wave
    float speed = 0.5f + (wave * 0.1f);  // Faster movement
    int tankChance = 10 + (wave * 5);    // More tanks later
    
    for (int i = 0; i < enemyCount; i++) {
        enemies.push_back(createEnemy(speed, tankChance));
    }
}
```

**Wave Progression:**
- Wave 1: 4 enemies, slow speed
- Wave 5: 8 enemies, moderate speed, some tanks
- Wave 10: 13 enemies, fast, many tanks

---

### Q: What power-ups exist?

**A:**

| Type | Effect | Duration |
|------|--------|----------|
| **Shield** | Takes 1 extra hit | Permanent (until hit) |
| **Rapid Fire** | 2x fire rate | 8 seconds |
| **Multi-Shot** | 3 bullets at once | 8 seconds |
| **Slow Motion** | Enemies 50% slower | 8 seconds |

---

### Q: How is the score calculated?

**A:**
```
Base Score = Enemy Type × Combo Multiplier

Enemy Type:
  - Weak:   10 points
  - Normal: 20 points
  - Tank:   50 points

Combo Multiplier:
  - Kill without being hit:  1.0x (resets on hit)
  - 2-5 kills:               1.2x
  - 5-10 kills:              1.5x
  - 10+ kills:               2.0x
```

**Example:** Kill 3 Tanks without getting hit = 50 × 3 × 1.5 = 225 points

---

## 🏗️ Scaffolding & File Organization

### Q: What files do I need to edit?

**A:**

| File | Edit When | Purpose |
|------|-----------|---------|
| `main.cpp` | Change game logic | Game mechanics, rendering, input |
| `CMakeLists.txt` | Add new library | Adding new dependencies |
| `highscores.txt` | Never (auto-generated) | Persistent score storage |

**Don't Edit:**
- `utils/` folder - Just dependencies
- `build/` folder - Compiler output

---

### Q: How do I add a new feature?

**A:** Example: Add new enemy type

1. **Define structure** (line ~115):
   ```cpp
   struct Enemy {
       float x, y;
       int type;  // Add new type number
       // ...
   };
   ```

2. **Update spawn logic** (line ~280):
   ```cpp
   if (rand() % 100 < newEnemyChance) {
       enemies.push_back({x, y, NEW_TYPE, speed, health});
   }
   ```

3. **Update collision** (line ~350):
   ```cpp
   if (enemy.type == NEW_TYPE) {
       score += 75;  // Unique score
   }
   ```

4. **Update rendering** (line ~600):
   ```cpp
   if (enemy.type == NEW_TYPE) {
       drawEnemySprite(enemy.x, enemy.y, NEW_TYPE);
   }
   ```

5. **Test** - Rebuild and verify

---

### Q: Where is the rendering code?

**A:** The `render()` function (lines 520-580) handles:

1. **Clear screen** - `glClear(GL_COLOR_BUFFER_BIT)`
2. **Draw player** - Triangle at player position
3. **Draw enemies** - Rectangles at enemy positions
4. **Draw bullets** - Small dots
5. **Draw UI** - Score, wave, health text

**Rendering Pipeline:**
```
Vertex data → GPU memory → Shader processing → Fragment colors → Screen
```

---

### Q: How do shaders work?

**A:** Shaders are GPU programs (in the string at line ~610):

```glsl
// Vertex Shader: Transforms 3D positions
#version 330
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(position, 0, 1);
}

// Fragment Shader: Determines pixel colors
#version 330
void main() {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red
}
```

**Why:** GPU processes millions of pixels in parallel; shaders tell it what color each pixel should be.

---

## 📝 File I/O & Persistence

### Q: How does high score saving work?

**A:**

**Load** (at startup):
```cpp
void loadHighScores(std::array<HighScore, 10>& scores) {
    std::ifstream file("highscores.txt");  // Open for reading
    for (int i = 0; i < 10; i++) {
        file >> scores[i].score >> scores[i].wave;  // Read each score
    }
    file.close();
}
```

**Save** (when game ends):
```cpp
void saveHighScores(const std::array<HighScore, 10>& scores) {
    std::ofstream file("highscores.txt");  // Open for writing
    for (int i = 0; i < 10; i++) {
        if (scores[i].score > 0) {
            file << scores[i].score << " " << scores[i].wave << "\n";
        }
    }
    file.close();
}
```

**File Format:**
```
score1 wave1
score2 wave2
...
score10 wave10
```

---

## 🐛 Debugging & Common Issues

### Q: How do I debug OpenGL errors?

**A:** Use `gl_debug()` macro (line 24):

```cpp
glDrawArrays(GL_TRIANGLES, 0, 6);
gl_debug(__FILE__, __LINE__);  // Print any GL errors
```

**Common errors:**
- `GL_INVALID_OPERATION` - Used function before context setup
- `GL_OUT_OF_MEMORY` - Too many vertices/textures for GPU
- `GL_INVALID_VALUE` - Invalid parameter (negative size, etc.)

---

### Q: How do I add debug output?

**A:** Use `printf()` or `fprintf()`:

```cpp
fprintf(stderr, "Spawning enemy at (%.1f, %.1f)\n", x, y);
fprintf(stderr, "Collision detected! Score: %d\n", score);
```

**Check console output** during game to see debug messages.

---

## 🚀 Next Steps / Extensions

### Q: What would make this project more modular in the future?

**A:** When expanding:

```
Option 1: Separate by concern
├── entities.h       (data structures)
├── physics.h        (collision, movement)
├── rendering.h      (draw functions)
├── input.h          (keyboard handling)
└── audio.h          (sound effects)

Option 2: Separate by layer
├── core/            (game logic)
├── graphics/        (rendering)
├── io/              (input/files)
└── utils/           (math, helpers)
```

**Benefits:** Easier to add features, reuse code, test independently

---

### Q: What features could be added?

**A:**
1. **Sound Effects** - Shoot, explosion, background music
2. **Menu System** - Start screen, pause menu, settings
3. **Levels** - Different game modes, boss battles
4. **Achievements** - Unlocks, badges
5. **Networking** - Multiplayer, leaderboard sync
6. **Better Graphics** - Sprite animation, particle effects
7. **Mobile Support** - Touch controls for phones

---

## 📖 Quick Reference

### Keybindings in Game:
- **Arrow Keys** - Move left/right
- **Space** - Shoot
- **P** - Pause
- **ESC** - Quit

### Build & Run:
```bash
cd c:\Study\Programming\PROJECTS\Game
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
cd ..
build\bin\space_invaders.exe
```

---

## 📚 Further Reading

- See `guides/` folder for setup, troubleshooting, and detailed docs
- See `guides/DOCS.md` for technical deep dives
- See `guides/ENHANCEMENTS.md` for feature ideas

