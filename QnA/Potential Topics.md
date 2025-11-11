# Potential Topics for Project Discussion

This document breaks down the Space Invaders project into key areas, providing potential questions and explanations that a professor might ask. The topics range from fundamental concepts to more advanced and critical-thinking questions.

---

### **Part I: Core Concepts & Setup**

#### 1. Project Overview
*   **Question:** What is the high-level goal of your project?
*   **Explanation:** The project is a 2D arcade-style game inspired by the classic "Space Invaders." The player controls a spaceship at the bottom of the screen and must shoot waves of descending enemies. The game includes modern features like a scoring system with combos, different enemy types, player power-ups, and a persistent high-score table.

#### 2. Libraries & Build System
*   **Question:** Your project uses `GLFW`, `GLEW`, and `CMake`. What role does each one play?
*   **Explanation:**
    *   **`CMake`:** This is the build system generator. It's not a compiler, but a tool that creates the actual project files (like Makefiles or Visual Studio solutions) needed to compile the code. It reads the `CMakeLists.txt` file to understand which source files to compile and how to link the necessary libraries.
    *   **`GLFW` (Graphics Library Framework):** This library handles operating system-specific tasks required for an OpenGL program. Its main jobs in this project are to create a window, create an OpenGL context for that window, and handle user input (like keyboard presses).
    *   **`GLEW` (OpenGL Extension Wrangler):** OpenGL features are often implemented directly by graphics card drivers. GLEW is a loading library that finds and loads the pointers to those modern OpenGL functions at runtime, making it possible to use them in the code without knowing the specific hardware beforehand.

#### 3. Program Entry Point
*   **Question:** Walk me through what happens in your `main()` function.
*   **Explanation:** The `main` function orchestrates the entire application lifecycle:
    1.  **Initialization:** It initializes `GLFW`, creates a window, and sets up the OpenGL context. It then initializes `GLEW` to load the necessary OpenGL functions.
    2.  **OpenGL State Setup:** It configures the initial OpenGL state, such as the viewport, projection matrix (`glOrtho`), and blending mode.
    3.  **Game Creation:** It creates an instance of the `GameState` struct, which holds all the data for the game session (player position, score, enemies, etc.).
    4.  **Game Loop:** It enters a `while` loop that continues as long as the window is open. Inside the loop, it calculates the time since the last frame (`deltaTime`), processes player input, updates the game logic, and renders everything to the screen.
    5.  **Cleanup:** Once the loop exits (the player closes the window or the game ends), it destroys the `GLFW` window and terminates the library to free up resources.

---

### **Part II: C++ Language & Structure**

#### 1. Data Structures
*   **Question:** You used `structs` like `GameState`, `Enemy`, and `Bullet`. Why choose a `struct` over a `class`? What are the alternatives?
*   **Explanation:**
    *   **`struct` vs. `class`:** In C++, the only technical difference is that `struct` members are `public` by default, while `class` members are `private`. I used `structs` here because these data containers primarily hold public data with very little complex logic. The `GameState` struct, for example, acts as a centralized container for all game data that needs to be accessed and modified by various functions. This is a more C-style, data-oriented approach.
    *   **Alternatives:** An alternative would be a more Object-Oriented (OOP) approach. Each game entity (`Player`, `Enemy`, `Bullet`) could be its own `class` with private member variables and public methods (e.g., `player.move()`, `enemy.takeDamage()`). The `GameState` could then become a manager class that owns and orchestrates these objects.

#### 2. Dynamic Collections
*   **Question:** What is `std::vector` and why did you use it for bullets and enemies? How does it manage memory?
*   **Explanation:**
    *   **What it is:** `std::vector` is a dynamic array from the C++ Standard Library. It can grow or shrink in size as needed.
    *   **Why it's used:** The number of bullets and enemies on screen is constantly changing. We don't know the maximum number of bullets ahead of time. A `std::vector` is perfect for this because we can easily add new bullets when the player shoots (`push_back`) and remove them when they go off-screen or hit something (`erase`).
    *   **Memory Management:** `std::vector` manages its own memory on the heap. When you add more elements than its current capacity, it automatically allocates a larger block of memory, copies the existing elements over, and then deletes the old block. This automates memory management, preventing common errors like memory leaks.

#### 3. File I/O
*   **Question:** How are high scores saved and loaded? Explain `ifstream` and `ofstream`.
*   **Explanation:**
    *   **`ofstream` (Output File Stream):** The `saveHighScores` function uses `std::ofstream` to write data *to* a file. It opens `highscores.txt`, iterates through the high scores array, and writes the score and wave for each entry as plain text.
    *   **`ifstream` (Input File Stream):** The `loadHighScores` function uses `std::ifstream` to read data *from* a file. It opens `highscores.txt` and reads the score and wave values, populating the in-memory `highScores` array.
    *   This provides persistence, allowing scores to be saved between game sessions.

#### 4. Functions & Lambdas
*   **Question:** Explain the purpose of a function like `spawnWave()` and your use of a lambda function in `update()`.
*   **Explanation:**
    *   **`spawnWave()`:** This function encapsulates the logic for creating a new wave of enemies. It clears the old enemies, calculates how many to create based on the current wave number, and sets their initial positions and types. This makes the code cleaner by separating the "how" of spawning enemies from the "when" (which is decided in the main update logic).
    *   **Lambda Function:** In the `update()` method, I use `std::remove_if` with a lambda function: `[](const Bullet& b) { return !b.active; }`.
        *   `std::remove_if` is an algorithm that shuffles all elements for which the lambda returns `true` to the end of the vector.
        *   The lambda is a small, anonymous function defined right where it's needed. This one takes a `Bullet` and returns `true` if the bullet is inactive.
        *   The full `erase-remove` idiom is a standard C++ way to efficiently remove elements from a container that meet a certain condition, without writing a manual loop.

---

### **Part III: Graphics with Immediate Mode OpenGL**

#### 1. The Graphics Pipeline (Legacy)
*   **Question:** Your code uses `glBegin`/`glEnd`. What is "Immediate Mode" and how does it differ from modern OpenGL?
*   **Explanation:**
    *   **Immediate Mode:** This is a deprecated, beginner-friendly way of sending drawing commands to the GPU. With `glBegin(GL_QUADS)` and `glVertex2f(...)`, the CPU is sending vertex data to the GPU one by one, for every single frame. It's called "immediate" because the commands are executed more or less right away.
    *   **Modern OpenGL:** Modern OpenGL (version 3.3+), in contrast, uses a shader-based pipeline. You first load all your vertex data into buffers on the GPU's memory (VBOs - Vertex Buffer Objects). Then, to draw something, you just tell the GPU to draw from that buffer. This is far more efficient because you aren't re-sending data every frame, which dramatically reduces the communication overhead between the CPU and GPU.

#### 2. Rendering Primitives
*   **Question:** How are the player, enemies, and bullets drawn on the screen?
*   **Explanation:** They are drawn using basic geometric shapes called "primitives."
    *   The player is a `GL_TRIANGLES` primitive.
    *   The enemies and bullets are `GL_QUADS` (four-sided polygons).
    *   The `glVertex2f` function specifies the 2D coordinates for each corner of the shape. The GPU then fills in the pixels between these vertices to draw the final shape on screen.

#### 3. Coordinate Systems & Transformations
*   **Question:** How does `glOrtho` work? How do you position objects?
*   **Explanation:**
    *   **`glOrtho`:** This function defines the coordinate system for the scene. `glOrtho(0, 640, 480, 0, ...)` sets up a 2D orthographic projection. It maps a 640x480 world space directly to the 640x480 pixel viewport. The top-left corner is `(0, 0)` and the bottom-right is `(640, 480)`. This makes it easy to reason about positions, as world coordinates map directly to pixel coordinates.
    *   **Positioning Objects:** Objects are positioned simply by providing their world coordinates in the `glVertex2f` calls. For example, to draw an enemy at `(e.x, e.y)`, I calculate the corner vertices relative to that center point (e.g., `e.x - 15`, `e.y - 15`).

#### 4. State Machine
*   **Question:** What does it mean that OpenGL is a "state machine"?
*   **Explanation:** OpenGL maintains a large "state" that affects all subsequent drawing commands. When you call a function like `glColor3f(1.0f, 0.0f, 0.0f)`, you are setting the "current color" state to red. Every shape drawn after that will be red, until you change the color state again. Other examples of state are the current matrix mode (`GL_PROJECTION`), blending function, and active texture. You have to carefully manage this state to get the results you want.

---

### **Part IV: Game Logic & Mechanics**

#### 1. The Game Loop
*   **Question:** Explain your `update()` and `render()` cycle. Why is `deltaTime` important?
*   **Explanation:**
    *   **The Cycle:** The main game loop continuously calls `handleInput()`, `update()`, and `render()`. This separates concerns: `handleInput` gathers player actions, `update` advances the game simulation (moves enemies, checks collisions), and `render` draws the current state to the screen.
    *   **`deltaTime`:** This variable stores the amount of time that has passed since the last frame. It is crucial for making movement and other time-based events frame-rate independent. For example, instead of moving an enemy by a fixed `1` pixel per frame, you move it by `speed * deltaTime`. This ensures the enemy moves at the same speed on a fast computer (many small updates) as on a slow one (fewer, larger updates).

#### 2. Input Handling
*   **Question:** How does the game process keyboard input for movement and shooting?
*   **Explanation:** In the `handleInput` function, I use `glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS`. This function polls the state of a key on every frame. If the left arrow key is currently being held down, the player's X-coordinate is decreased. For shooting, I check if the spacebar is pressed and also check a timer (`lastShootTime`) to implement a cooldown, preventing the player from firing too rapidly (unless they have the rapid-fire power-up).

#### 3. Collision Detection
*   **Question:** How does the game know when a bullet hits an enemy? What are the limitations of this approach?
*   **Explanation:**
    *   **How it works:** The game uses an Axis-Aligned Bounding Box (AABB) method. It treats every object (bullet, enemy, player) as a simple rectangle. A collision is detected by checking if the rectangular bounds of a bullet overlap with the rectangular bounds of an enemy. This is done with a series of simple `if` statements comparing their x and y coordinates.
    *   **Limitations:** This method is very fast but not very precise. It works well for square or rectangular objects. However, if the visual shape of an object is, for example, a triangle or a circle, the rectangular "hitbox" will not match it perfectly. This can lead to situations where a collision is detected with the empty space in the corner of the hitbox.

#### 4. Game State
*   **Question:** How are the `paused` and `gameOver` states managed?
*   **Explanation:** These are managed with simple boolean flags (`paused`, `gameOver`) in the `GameState` struct.
    *   When the `P` key is pressed, the `paused` flag is toggled. The `update()` and `handleInput()` functions check this flag at the very beginning, and if it's `true`, they immediately `return`, effectively freezing the game logic.
    *   The `gameOver` flag is set to `true` when the player's `lives` reach zero or an enemy reaches the bottom of the screen. When the main loop sees this flag is `true`, it breaks out, prints the final score, and ends the game.

#### 5. Advanced Mechanics
*   **Question:** Explain the implementation of your power-up system.
*   **Explanation:**
    1.  **Spawning:** There's a chance a power-up will spawn when an enemy is defeated. A `PowerUp` struct is created with a random type and added to a `std::vector`.
    2.  **State:** The `GameState` struct contains timer variables for each power-up (e.g., `rapidFireActive`, `shieldActive`).
    3.  **Activation:** When the player collides with a power-up object, the corresponding timer in `GameState` is set to a positive value (e.g., `rapidFireActive = 8.0f` for 8 seconds).
    4.  **Effect:** The game logic checks these timers to alter behavior. For example, the shooting code checks if `rapidFireActive > 0` to decrease the cooldown between shots. The player rendering code checks if `shieldActive > 0` to draw a shield.
    5.  **Deactivation:** In the `update` function, each active timer is decreased by `deltaTime`. When a timer reaches zero, the power-up effect expires.

---

### **Part V: Advanced & "Tricky" Questions**

#### 1. Performance
*   **Question:** Your code uses `std::vector::erase(std::remove_if(...))`. Why is this pattern used? Is it efficient?
*   **Explanation:**
    *   **Why it's used:** This is the standard "erase-remove idiom" in C++. Simply calling `vector::erase` inside a loop is problematic because it invalidates iterators and can be very slow, as it shuffles all subsequent elements down one by one for every single removal.
    *   **Efficiency:** The `std::remove_if` part is efficient. It iterates through the vector once, moving all elements that are *not* being removed to the front. It doesn't actually remove anything; it just returns an iterator to the new "logical" end of the vector. Then, the single call to `erase` removes all the "dead" elements at the end in one operation. This is much more efficient than repeated single erasures.

#### 2. Immediate Mode vs. Modern OpenGL
*   **Question:** If you were to rewrite the rendering, what would be the benefits of using shaders, VBOs, and VAOs?
*   **Explanation:**
    *   **Performance:** The biggest benefit is performance. By storing vertex data in a **VBO** (Vertex Buffer Object) on the GPU, you eliminate the need to send it from the CPU every frame. A **VAO** (Vertex Array Object) then stores the configuration for that VBO (how the data is structured), so a draw call becomes a simple command to "draw this pre-configured object," which is extremely fast.
    *   **Flexibility:** **Shaders** (programs that run on the GPU) would give me complete control over the graphics pipeline. Instead of being limited to `glColor3f`, I could implement complex visual effects like lighting, particle effects for explosions, or screen-wide post-processing effects like bloom or screen shake. Immediate mode is fixed-function and cannot do these things.

#### 3. Bugs & Edge Cases
*   **Question:** What could happen if too many bullets are on screen? What if the frame rate drops significantly?
*   **Explanation:**
    *   **Too many bullets:** Each bullet requires memory and processing time for updates and collision checks. If thousands of bullets were on screen, the `std::vector` holding them would grow very large, potentially causing a small memory spike. More importantly, the nested loops for collision detection (`for each bullet` -> `for each enemy`) would become very slow, causing the frame rate to drop.
    *   **Frame Rate Drop:** My game logic is dependent on `deltaTime`, so the game speed itself should remain consistent even if the frame rate is low. However, the player experience would suffer. Input would feel laggy, and animations would appear choppy. A severe drop could also cause "tunneling" in collision detection, where a fast-moving object moves so far in one frame that it completely passes through another object without an overlap ever being detected.

#### 4. Code Refinements
*   **Question:** The entire game is in one file. How would you refactor this into multiple files and classes? What would be the benefits?
*   **Explanation:**
    *   **How to Refactor:**
        1.  **Entity Classes:** I would create separate header/source files for each major game entity (`Player.h`/`.cpp`, `Enemy.h`/`.cpp`, `Bullet.h`/`.cpp`). These classes would contain their own data and logic (e.g., an `Enemy` class would have its own `update()` and `render()` methods).
        2.  **Manager Classes:** I would create manager classes like `EntityManager` to handle collections of entities (updating and rendering all of them) and a `Game` class to replace the `GameState` struct and manage the main loop and game states.
        3.  **Renderer Class:** The OpenGL drawing code could be moved into its own `Renderer` class, separating the game logic from how it's drawn.
    *   **Benefits:**
        *   **Readability & Maintenance:** It's much easier to find and understand code when it's organized logically.
        *   **Reusability:** A `Player` class could potentially be reused in another game.
        *   **Decoupling:** Separating logic from rendering would make it easier to switch to a different graphics library in the future without rewriting the entire game.

#### 5. Memory Management
*   **Question:** The code uses `std::vector`, which handles memory automatically. What would be the challenges if you had to manage the memory for bullets and enemies manually with `new` and `delete`?
*   **Explanation:**
    *   **Challenges of Manual Management:**
        1.  **Memory Leaks:** For every `new Bullet()`, I would need to ensure there is a corresponding `delete`. If a bullet goes off-screen and I forget to `delete` it, that memory is leaked and can never be used again.
        2.  **Dangling Pointers:** If I `delete` a bullet but another part of the code still holds a pointer to it and tries to use it, the program will crash or have undefined behavior.
        3.  **Complexity:** I would have to write my own dynamic array or linked list structure to hold the pointers, and manually manage resizing and cleanup. This is complex and error-prone.
    *   Using `std::vector` and smart pointers (like `std::unique_ptr` or `std::shared_ptr`) abstracts away these dangers, which is why modern C++ strongly encourages their use. It follows the principle of RAII (Resource Acquisition Is Initialization).