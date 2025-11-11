# Foundational Questions

This file contains questions and answers related to the foundational aspects of the project, including the setup, libraries, and core C++ structure.

---

### **Part 1: Headers, Libraries & Initialization**

**Q: What is the purpose of including `<cstdio>`?**
**A:** The `<cstdio>` header is included for standard C input/output operations. In this code, it's specifically used for `fprintf(stderr, ...)` in the `gl_debug` function and for `printf` and `fflush` in the main game loop to print stats to the console.

**Q: What are the roles of `glew.h` and `glfw3.h`?**
**A:**
*   **`glfw3.h` (GLFW):** This library is responsible for creating and managing the window, the OpenGL context, and user input (like keyboard presses). It's the interface between the code and the operating system's windowing system.
*   **`glew.h` (GLEW):** This library handles loading OpenGL function pointers from the graphics driver. It allows the use of modern OpenGL functions that aren't part of the base standard. It must be included after the OpenGL headers (which GLFW includes).

**Q: Why do you have to call `glewInit()`? And why must it be *after* `glfwMakeContextCurrent()`?**
**A:** `glewInit()` initializes the GLEW library by loading pointers to the OpenGL functions. This can only be done after a valid OpenGL context has been created and made active on the current thread. `glfwMakeContextCurrent()` is the function that activates this context. Without an active context, there are no functions for GLEW to load, and it will fail.

**Q: In your `main` function, you call `glfwWindowHint`. What does this function do?**
**A:** `glfwWindowHint` is used to configure the properties of the window *before* it is created. In this project, it's used to specify the desired OpenGL version (3.3) and profile (Compatibility Profile), ensuring the program gets a rendering context with the features it expects.

**Q: What does `glfwSwapInterval(1);` do?**
**A:** This function enables V-Sync (Vertical Sync). It synchronizes the application's frame rate with the monitor's refresh rate. An interval of `1` means the application will wait for one screen refresh to pass before swapping the buffers, effectively capping the frame rate at the monitor's refresh rate (e.g., 60 FPS) and preventing screen tearing.

---

### **Part 2: Core C++ Structure & Data**

**Q: Why are `Bullet`, `PowerUp`, and `Enemy` defined as `structs` instead of `classes`?**
**A:** In C++, `structs` and `classes` are nearly identical, but `struct` members are `public` by default. I used `structs` here because these are primarily simple data containers. Their main purpose is to hold data (`x`, `y`, `active`, `health`, etc.) that is accessed directly by the main game logic, rather than encapsulating complex behavior with private members and public methods.

**Q: What is `std::array` and why did you choose it over `std::vector` for `highScores`?**
**A:** `std::array` is a container for a fixed-size sequence of elements. I chose it for the high scores because the number of high scores is constant (always 10). Unlike `std::vector`, `std::array` has a fixed size determined at compile time, which makes it slightly more memory-efficient as it doesn't need to store extra information about capacity or allow for resizing.

**Q: What is an initializer list (`: playerX(320), ...`) and why is it used in the `GameState` constructor?**
**A:** An initializer list is a more efficient way to initialize member variables in a constructor. It initializes the members directly, whereas assigning values inside the constructor body (`{ playerX = 320; }`) would first default-initialize them and then assign a new value. For simple types like `int` and `float` the difference is negligible, but for complex objects, using an initializer list is a best practice.

**Q: Explain the `std::remove_if` and `.erase()` combination you use in the `update` function.**
**A:** This is the "erase-remove idiom" in C++, which is the standard way to efficiently delete multiple elements from a `std::vector`.
1.  `std::remove_if` iterates through the vector and moves all elements that should *not* be deleted to the beginning of the vector. It returns an iterator pointing to the new "logical" end.
2.  `.erase()` is then called once to delete all the "inactive" elements from that new logical end to the actual end of the vector.
This is much faster than deleting elements one by one inside a loop, which would require shuffling the remaining elements down after every single deletion.

---

### **Part 3: File I/O and Debugging**

**Q: How are high scores saved to and loaded from "highscores.txt"?**
**A:**
*   **Loading (`loadHighScores`):** Uses `std::ifstream` (input file stream) to open and read from the file. It reads the score and wave numbers line by line and populates the `highScores` array.
*   **Saving (`saveHighScores`):** Uses `std::ofstream` (output file stream) to open and write to the file. It iterates through the `highScores` array and writes the score and wave for each entry.

**Q: What is the purpose of the `gl_debug` function?**
**A:** `gl_debug` is a helper function for debugging. It calls `glGetError()` in a loop to check if OpenGL has recorded any errors from previous operations. If an error is found, it converts the error code into a human-readable string and prints it to the standard error stream (`stderr`), along with the file and line number where the check was performed. This is extremely useful for catching and fixing graphics-related bugs.