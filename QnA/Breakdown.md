# Breakdown of `main.cpp`

This document explains the entire `main.cpp` file in a simple, easy-to-understand way. It's broken down into logical sections that match the flow of the code.

---

### **1. Overview**

At a high level, this single `main.cpp` file contains a complete 2D Space Invaders-style game. It uses C++ and a few key libraries:
*   **GLFW:** To create a window, handle keyboard input, and manage the OpenGL context.
*   **GLEW:** To access modern OpenGL functions provided by the graphics driver.
*   **OpenGL (Immediate Mode):** To draw all the graphics (player, enemies, bullets) on the screen.

The code is structured in a procedural way, with most of the game's data and logic contained within a large `GameState` struct.

---

### **2. The Building Blocks: Data Structures (`structs`)**

Before the main logic, several `structs` are defined. Think of these as simple blueprints for the objects in our game.

*   `struct Bullet`: Represents a single projectile. It just needs to know its `x` and `y` position and whether it's `active` (visible and moving) or not.
*   `struct PowerUp`: Represents a collectible item. It has a position, a `type` (to know if it's a shield, rapid-fire, etc.), and an `active` flag.
*   `struct Enemy`: Represents a single enemy ship. It has a position, a `type` (weak, normal, or tank), its current `health`, and an `active` flag.

The `active` flag is a simple and effective way to "delete" objects. Instead of actually removing them from memory mid-game (which can be slow), we just mark them as inactive. Later, a cleanup process removes all inactive objects at once.

---

### **3. The Brain of the Game: The `GameState` Struct**

This is the most important data structure in the entire program. It acts as a central container for **all** the information about the current game session. This includes:

*   **Player Data:** `playerX`, `playerY`, `lives`.
*   **Object Collections:** `std::vector` lists to hold all active `playerBullets`, `enemyBullets`, `enemies`, and `powerUps`. Using `std::vector` is convenient because it automatically handles memory as bullets and enemies are created and destroyed.
*   **Game Status:** `score`, `wave`, `gameOver`, and `paused` flags to control the state of the game.
*   **Difficulty & Timers:** Variables to manage enemy movement speed, power-up durations, and combo multipliers.
*   **High Scores:** An array to hold the top 10 high scores, which are loaded from and saved to a file.

By keeping everything in one place, we can easily pass all the game's information to our functions.

---

### **4. Core Gameplay Logic: Key Functions**

The `GameState` struct also contains the four functions that control the entire game flow.

#### `spawnWave()`
*   **Purpose:** To set up the enemies for a new level.
*   **How it works:** It is called at the start of the game and whenever the player clears a wave. It clears out any old enemies and then uses two `for` loops to create a new grid of enemies. The number of rows and the types of enemies it creates depend on the current `wave` number, making the game harder as the player progresses.

#### `update(float dt)`
*   **Purpose:** To advance the game simulation by one step. `dt` (deltaTime) is the time that has passed since the last frame, ensuring the game runs at the same speed on all computers.
*   **How it works:** This is where most of the game logic happens. On every frame, it does the following:
    1.  **Moves Enemies:** It moves the block of enemies left or right. If an enemy hits the edge of the screen, it reverses their direction and moves them all down.
    2.  **Updates Bullets:** It moves all player and enemy bullets.
    3.  **Checks Collisions:** It checks if any player bullets have hit an enemy or if any enemy bullets have hit the player. This is done with simple rectangle-based collision detection.
    4.  **Updates Score & Health:** If a collision happens, it updates the enemy's health, the player's score, and the combo counter.
    5.  **Handles Power-ups:** It moves power-ups and checks if the player has collected one. It also counts down the timers for any active power-ups.
    6.  **Enemy Shooting:** It gives enemies a chance to shoot, with the chance increasing on higher waves.
    7.  **Cleanup:** At the end, it removes all "inactive" bullets, enemies, and power-ups from their respective `std::vector` lists using the `erase-remove` idiom.

#### `handleInput(GLFWwindow* window)`
*   **Purpose:** To check for and respond to player input.
*   **How it works:** On every frame, this function checks if the player is pressing any keys:
    *   **A/D or Left/Right Arrows:** Moves the player's ship left or right.
    *   **Spacebar:** Fires a bullet, but only if enough time has passed since the last shot (the "cooldown"). This logic is modified by the rapid-fire and multi-shot power-ups.
    *   **P Key:** Toggles the `paused` flag.

#### `render()`
*   **Purpose:** To draw everything onto the screen.
*   **How it works:** This function is purely for visuals. It clears the screen to black and then goes through the data in `GameState` to draw everything in its current position. It uses basic, immediate-mode OpenGL commands (`glBegin`, `glEnd`, `glVertex2f`) to draw simple shapes like triangles (for the player) and quads (for enemies and bullets). It uses `glColor3f` to change the color before drawing each type of object.

---

### **5. Tying It All Together: The `main()` Function**

This is the entry point of the program. It sets everything up and runs the game.

1.  **Initialization:** It initializes the `GLFW` and `GLEW` libraries, creates the game window, and sets up a basic 2D coordinate system with `glOrtho`.
2.  **Game Creation:** It creates the `GameState` object, which automatically calls its constructor to load high scores and spawn the first wave.
3.  **The Game Loop:** This is the `while (!glfwWindowShouldClose(window))` loop that runs continuously. On each iteration, it:
    *   Calculates the `deltaTime`.
    *   Calls `game.handleInput()` to process player actions.
    *   Calls `game.update()` to advance the game's logic.
    *   Calls `game.render()` to draw the new frame.
    *   Calls `glfwSwapBuffers()` to display the newly drawn frame to the user.
    *   Calls `glfwPollEvents()` to check for window events (like the user clicking the close button).
4.  **Game Over:** If the `gameOver` flag in `GameState` becomes `true`, the loop breaks. The final score is printed to the console, the high score list is updated, and the game ends.
5.  **Cleanup:** After the loop finishes, it calls `glfwDestroyWindow()` and `glfwTerminate()` to safely close the application and free up system resources.

---

### **6. Helper Utilities**

*   **High Score Functions:** `loadHighScores` and `saveHighScores` handle reading from and writing to the `highscores.txt` file, allowing the player's scores to persist between game sessions.
*   **`gl_debug`:** A small utility function used during development to ask OpenGL if any errors have occurred and print them to the console. This helps in debugging graphics issues.