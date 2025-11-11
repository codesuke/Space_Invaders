# Potential Questions & Answers about main.cpp

This file provides a detailed Q&A format for specific questions about the `main.cpp` code, designed to help you prepare for a technical discussion about the project.

---

### **Part I: Setup & Libraries**

**Q: In your `main` function, you call `glfwWindowHint`. What does this function do?**
**A:** `glfwWindowHint` is used to configure the properties of the window that will be created *before* it's actually created. In my code, I use it to specify that I want to use the OpenGL Compatibility Profile and request a specific version of OpenGL (3.3). This ensures that I get an OpenGL context with the features I expect.

**Q: Why do you have to call `glewInit()`? And why does it have to be *after* `glfwMakeContextCurrent()`?**
**A:** `glewInit()` is the function that initializes the GLEW library. This library is responsible for loading pointers to the OpenGL functions provided by the graphics driver. This can only be done after an OpenGL context has been created and made active, which is what `glfwMakeContextCurrent()` does. If you call `glewInit()` before you have an active context, it will fail because there are no OpenGL functions to load yet.

**Q: What is the difference between `glfwPollEvents()` and `glfwWaitEvents()`? Why did you choose `poll`?**
**A:** `glfwPollEvents()` checks for any pending events (like keyboard presses or mouse movement) and immediately returns. `glfwWaitEvents()` does the same, but if there are no events, it will pause the thread and wait until one occurs. For a game, you must use `glfwPollEvents()`. The game needs to keep running and updating the screen even if the user isn't doing anything. If I used `glfwWaitEvents()`, the entire game would freeze until I pressed a key or moved the mouse.

---

### **Part II: C++ & Code Structure**

**Q: In your `GameState` struct, why are variables like `playerX` floats instead of integers?**
**A:** I use `float` for positions to allow for smooth, non-integer movement. While the screen is made of integer pixels, game logic often uses a `deltaTime` value (a fraction of a second) for updates. Multiplying a speed value by `deltaTime` results in a fractional change. Using floats allows this movement to accumulate accurately over time, resulting in smoother motion regardless of the frame rate.

**Q: Explain your `spawnWave` function. What would happen if you changed the `cols` variable from 6 to 10?**
**A:** The `spawnWave` function is responsible for creating all the enemies for a new wave. It clears any old enemies, calculates the number of rows based on the wave number, and then uses nested loops to create enemies in a grid pattern. If I changed `cols` from 6 to 10, the grid of enemies would become wider, with 10 enemies in each row instead of 6.

**Q: In `insertHighScore`, what is the purpose of the loop `for (int j = 9; j > i; j--)`?**
**A:** That loop is responsible for making space for a new high score. When a new score is higher than a score at index `i`, this loop shifts all the lower scores down by one position. It starts from the bottom of the list (`j=9`) and copies the score from the position above it (`scores[j] = scores[j-1]`) until it reaches the position `i` where the new score needs to be inserted.

**Q: How would you change the high score list to store 20 scores instead of 10?**
**A:** I would need to make changes in a few places:
1.  Change the `std::array<HighScore, 10>` to `std::array<HighScore, 20>` in the `GameState` struct and in the function signatures for `loadHighScores`, `saveHighScores`, and `insertHighScore`.
2.  Update the loops in those functions to iterate up to 20 instead of 10.
3.  In `insertHighScore`, the loop that shifts scores down should start from `j = 19`.

---

### **Part III: Graphics & Rendering**

**Q: What does `glOrtho(0, 640, 480, 0, -1, 1)` mean, especially the `480, 0` part? What happens if you swap them to `0, 480`?**
**A:** `glOrtho` sets up the coordinate system. The first four parameters are left, right, bottom, and top. By setting it to `(0, 640, 480, 0)`, I am defining a 640x480 space where the coordinate `(0,0)` is at the **top-left** corner of the screen. This is common in 2D graphics. If I swapped the last two numbers to `(0, 640, 0, 480)`, the `(0,0)` coordinate would be at the **bottom-left**, which is the traditional Cartesian system. All my game objects would appear upside down until I updated their Y-coordinates accordingly.

**Q: In the `render` function, you set the color with `glColor3f` before drawing each object type. What would happen if you only set the color once at the beginning of the function, for example to green?**
**A:** OpenGL is a state machine. If I set the color to green once at the beginning of `render()`, every single object drawn after that—the player, all the enemies, and all the bullets—would be drawn in green. The `glColor3f` call sets the "current color," and that state persists until it's changed again.

**Q: How would you change the player's ship from a green triangle to a blue square?**
**A:** In the `render()` function, I would find the section for drawing the player.
1.  Change `glColor3f(0.0f, 1.0f, 0.0f);` (green) to `glColor3f(0.0f, 0.0f, 1.0f);` (blue).
2.  Change `glBegin(GL_TRIANGLES);` to `glBegin(GL_QUADS);`.
3.  Replace the three `glVertex2f` calls for the triangle with four `glVertex2f` calls that define a square, for example:
    ```cpp
    glVertex2f(playerX - 20, playerY - 20);
    glVertex2f(playerX + 20, playerY - 20);
    glVertex2f(playerX + 20, playerY + 20);
    glVertex2f(playerX - 20, playerY + 20);
    ```

**Q: Your shield is a cyan outline around the player. How is that effect achieved?**
**A:** When the `shieldActive` timer is greater than zero, I draw the player's ship shape using `glBegin(GL_LINE_LOOP)` instead of `GL_TRIANGLES`. `GL_LINE_LOOP` connects the vertices with lines but doesn't fill the shape, creating an outline. I also make the vertices slightly larger than the actual player ship to make the shield appear around it.

---

### **Part IV: Game Logic & Mechanics**

**Q: In the `update` function, what is `enemyDirection` used for? How does the game detect that the enemies have hit the edge of the screen?**
**A:** `enemyDirection` is a float that is either `1.0f` (move right) or `-1.0f` (move left). In the `update` function, I check if any active enemy's x-position has gone past the left (`< 20`) or right (`> 600`) boundary. If one has, a flag `hitEdge` is set to true. After checking all enemies, if `hitEdge` is true, I multiply `enemyDirection` by -1 to reverse the movement direction for the entire group and move all enemies down one step.

**Q: How does the rapid-fire power-up work? If I wanted to make it fire 3x faster instead of 2x, what value would I change?**
**A:** When the player collects the power-up, a timer `rapidFireActive` is set to 8 seconds. In the input handling code for shooting, there's a `cooldown` variable, which is normally `0.2f`. The code checks `if (rapidFireActive > 0)`. If it is, the `cooldown` is reduced to `0.1f`, doubling the fire rate. To make it 3x faster, I would change that line to `cooldown = 0.2f / 3.0f;`.

**Q: I want to add a new power-up that gives the player an extra life. How would I do that? Where would the code go?**
**A:** Here are the steps:
1.  **Add a new power-up type:** In the `PowerUp` struct, I'd add a comment to remember the new type, e.g., `// 4=extralife`.
2.  **Update spawning:** In `spawnWave`, when a power-up is created, I'd change `p.type = rand() % 4;` to `p.type = rand() % 5;` to include the new type.
3.  **Update rendering:** In `render()`, I'd add a `case 4:` to the `switch` statement for power-ups to give the extra life power-up a unique color.
4.  **Implement the effect:** In `update()`, inside the power-up collision check, I'd add a `case 4:` to the `switch` statement. The code inside would simply be `lives++;` and maybe a check to not go over a maximum number of lives.

---

### **Part V: Advanced & "What If" Scenarios**

**Q: The game loop has `if (deltaTime < targetFrameTime) { continue; }`. What is this for? What would happen if you removed it?**
**A:** This code is a simple frame rate limiter. `targetFrameTime` is set to `1.0 / 60.0`, which is about 16.6 milliseconds. This `if` statement checks if the time since the last frame is less than the target. If it is, `continue` skips the rest of the loop, effectively pausing until enough time has passed. If I removed it, the game would run as fast as the computer could possibly render it (a "runaway loop"), potentially hundreds or thousands of frames per second. This would waste a lot of CPU/GPU power and could cause strange physics behavior on some systems.

**Q: In the enemy shooting logic, you have `int shootChance = 5 + wave * 2;`. What is the effect of this formula? What would happen if a player got to wave 50?**
**A:** This formula controls the shooting frequency of the enemies, making the game harder over time. The base chance for an enemy to shoot is 5%, and this chance increases by 2% with each wave. At wave 50, the `shootChance` would be `5 + 50 * 2 = 105`. Since the check is `rand() % 100 < shootChance`, this means that after wave 48, every enemy would have a 100% chance to shoot on every check, making the screen almost impossible to survive. This suggests the difficulty scaling might be too aggressive for very high waves and could be capped.

**Q: The game uses `printf` to display the score and a "PAUSED" message. Why does this output to the console and not the game window? How would you render text inside the game window itself?**
**A:** `printf` is a standard C function that writes formatted text to the standard output stream, which is typically the console or terminal that launched the application. It has no connection to the OpenGL window. To render text *inside* the game window, you need a proper font rendering library (like FreeType or stb_truetype) or a simpler "bitmap font" approach. With a bitmap font, you'd have a texture containing all the characters, and you would render the score by drawing a series of quads, each textured with the appropriate character from the image.

**Q: If you wanted to add sound effects, where in the code would you trigger the "player shoot" sound and the "enemy hit" sound?**
**A:** Assuming I had a sound library with a `playSound("filename.wav")` function:
*   **Player Shoot:** I would call `playSound("shoot.wav")` inside the `handleInput` function, right after a bullet is successfully created when the player presses the spacebar.
*   **Enemy Hit:** I would call `playSound("hit.wav")` inside the `update` function, within the bullet-enemy collision detection logic, right after a bullet hits an enemy and `e.health--` is called. If `e.health` becomes 0, I could play a different, louder `playSound("explosion.wav")`.