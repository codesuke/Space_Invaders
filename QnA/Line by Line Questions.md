# Potential Questions (Line-by-Line)

This file lists potential questions about the `main.cpp` code, ordered to follow the code from top to bottom.

---

### **Headers and Initial Setup (Lines 1-11)**

1.  What is the purpose of including `<cstdio>`?
2.  What are the roles of `glew.h` and `glfw3.h`? Why are they included in that order?
3.  Why do you need `<vector>`, `<cmath>`, `<algorithm>`, and the other standard library headers?
4.  What does the `GL_ERROR_CASE` macro do? How does the `#glerror` stringification work?
5.  What is a forward declaration and why is `struct HighScore` declared this way?

### **`gl_debug` Function (Lines 13-29)**

6.  What is the purpose of this `gl_debug` function? When should it be called?
7.  How does `glGetError()` work? Why is it in a `while` loop?
8.  What does `fprintf(stderr, ...)` do? How is it different from `printf`?

### **High Score Management (Lines 31-70)**

9.  What is `std::array` and why did you choose it over `std::vector` for `highScores`?
10. In `loadHighScores`, why do you initialize all scores to zero before reading the file?
11. What does `file.is_open()` check for? What happens if "highscores.txt" doesn't exist?
12. In `saveHighScores`, why do you check `if (scores[i].score > 0)` before writing to the file?
13. Walk me through the logic of `insertHighScore`. What would happen if two players had the exact same high score?

### **Game Data Structures (Lines 72-103)**

14. Why are `Bullet`, `PowerUp`, and `Enemy` defined as `structs` instead of `classes`?
15. What is the purpose of the `active` boolean in these structs?
16. In the `Enemy` struct, what is the difference between `type` and `health`?
17. The `GameState` struct contains many `std::vector` members. What is the advantage of this?
18. What are the `shieldActive`, `rapidFireActive`, etc., timers for? Why are they floats?

### **`GameState::GameState()` Constructor (Lines 105-112)**

19. What is an initializer list (`: playerX(320), ...`) and why is it used here?
20. Why is it important to call `loadHighScores()` and `spawnWave()` in the constructor?

### **`GameState::spawnWave()` (Lines 114-156)**

21. How does the difficulty scale with each wave in this function?
22. Explain the logic for determining the enemy `type` based on the `wave` number. What happens at wave 1 vs. wave 10?
23. Why is an enemy's `health` set based on its `type`?

### **`GameState::update()` (Lines 158-307)**

24. What is the first thing this function checks? Why?
25. What is the `dt` parameter and why is it important for game updates?
26. How does the enemy movement speed change based on the wave number? What does `moveSpeed = moveSpeed < 0.3f ? 0.3f : moveSpeed;` do?
27. Explain the logic for how the block of enemies moves and changes direction.
28. In the player bullet update loop, how is collision with an enemy detected? What are the limitations of this AABB collision detection?
29. How does the combo system work? What do `comboCounter` and `comboMultiplier` do?
30. What is the probability that a power-up will spawn when an enemy is defeated? How would you change this to 50%?
31. How does the player collect a power-up? What happens when they do?
32. When the player is hit by an enemy bullet, what logic determines if they lose a life or if the shield absorbs the hit?
33. How are the power-up timers updated? What happens when `rapidFireActive` becomes less than or equal to 0?
34. How does the enemy shooting logic change based on the wave number and enemy type?
35. What does the `std::remove_if` and `.erase()` combination do for the `playerBullets` vector? Why is this better than manually looping and erasing elements?
36. What condition causes the `wave` to increase?

### **`GameState::handleInput()` (Lines 309-372)**

37. How does the pause functionality work? Why is the `pKeyPressed` static variable necessary?
38. How do you prevent the player from moving off-screen?
39. Explain the shooting cooldown logic. How does the `rapidFireActive` power-up affect it?
40. How is the `multiShotActive` power-up implemented?

### **`GameState::render()` (Lines 374-483)**

41. What does `glClear(GL_COLOR_BUFFER_BIT)` do?
42. What is `glLoadIdentity()` used for?
43. How do you render the shield effect? Why is `GL_LINE_LOOP` used?
44. How are different enemy types given different colors?
45. How do you indicate that a "Tank" enemy has been damaged but not destroyed?
46. Why is the pause indicator (`printf`) sent to the console instead of being drawn on the screen?

### **`main()` Function (Lines 485-586)**

47. What is the purpose of `srand((unsigned int)time(0));`? What would happen if you removed it?
48. What does `glfwSwapInterval(1);` do?
49. What does the `glVersion` array and the associated `glGetIntegerv` calls do?
50. What is the purpose of `glViewport`?
51. Explain the line `glOrtho(0, 640, 480, 0, -1, 1);`. What would happen if you changed `480, 0` to `0, 480`?
52. What do `glEnable(GL_BLEND)` and `glBlendFunc(...)` do?
53. The main loop uses a `deltaTime` calculation. Why is this better than just running the loop as fast as possible?
54. What is the purpose of the line `if (deltaTime < targetFrameTime) { continue; }`?
55. What does `fflush(stdout);` do, and why is it necessary after the `printf` that shows the score?
56. How does the code check if a new high score was achieved after the game is over?
57. What is the difference between `glfwSwapBuffers(window);` and `glClear(GL_COLOR_BUFFER_BIT);`?
58. What is the purpose of `glfwDestroyWindow` and `glfwTerminate`? What could happen if you forgot to call them?
