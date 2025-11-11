# Space Invaders - Code Breakdown & Customization Guide

*Complete breakdown of functions, features, and how to modify everything*

---

## 📋 Table of Contents

1. [Code Structure](#-code-structure)
2. [Data Structures](#-data-structures)
3. [Function Breakdowns](#-function-breakdowns)
4. [Feature Breakdowns](#-feature-breakdowns)
5. [Customization Guide](#-customization-guide)
6. [Color & Graphics Guide](#-color--graphics-guide)
7. [Speed & Timing Guide](#-speed--timing-guide)
8. [Advanced Modifications](#-advanced-modifications)

---

## 🏗️ Code Structure

### Project Organization

```
main.cpp (690 lines)
├── Includes (1-12)
│   └── Standard C++, OpenGL, GLFW, GLEW
│
├── Macros & Debug (14-38)
│   └── GL_ERROR_CASE, gl_debug()
│
├── Data Structures (40-135)
│   ├── HighScore struct
│   ├── Bullet struct
│   ├── PowerUp struct
│   ├── Enemy struct
│   └── GameState class
│
├── Helper Functions (40-90)
│   ├── loadHighScores()
│   ├── saveHighScores()
│   └── insertHighScore()
│
├── GameState Methods (132-650)
│   ├── spawnWave()
│   ├── update()
│   ├── handleInput()
│   └── render()
│
├── Shader Code (602-650)
│   └── Vertex & Fragment shaders (as strings)
│
└── Main Function (652-690)
    └── Initialization & Game Loop
```

---

## 📊 Data Structures

### 1. HighScore Struct (Lines 16-18)

```cpp
struct HighScore {
    int score;  // Final score
    int wave;   // Wave number reached
};
```

**Purpose:** Store high score information
**Used in:** Score system, persistence

---

### 2. Bullet Struct (Lines 92-95)

```cpp
struct Bullet {
    float x, y;    // Position
    bool active;   // Is it alive?
};
```

**Purpose:** Represent both player and enemy bullets
**Used in:** Shooting system, collision detection

---

### 3. PowerUp Struct (Lines 97-102)

```cpp
struct PowerUp {
    float x, y;   // Position
    int type;     // 0=shield, 1=rapidfire, 2=multishot, 3=slowmotion
    bool active;  // Is it alive?
};
```

**Purpose:** Store power-up instances
**Used in:** Power-up system, collection

---

### 4. Enemy Struct (Lines 104-110)

```cpp
struct Enemy {
    float x, y;      // Position
    int type;        // 0=weak, 1=normal, 2=tank
    int health;      // Hits to destroy
    bool active;     // Is it alive?
};
```

**Purpose:** Represent enemy entities
**Used in:** Enemy system, collision detection, rendering

**Type breakdown:**
- Type 0 (Weak): 10 points, yellow, 1 hit to kill
- Type 1 (Normal): 20 points, red, 1 hit to kill
- Type 2 (Tank): 50 points, dark red, 3 hits to kill

---

### 5. GameState Class (Lines 112-650)

**Main class holding all game logic and state**

**Key Variables:**
```cpp
float playerX, playerY;              // Player position
std::vector<Bullet> playerBullets;   // Player shots
std::vector<Bullet> enemyBullets;    // Enemy shots
std::vector<Enemy> enemies;          // All enemies
std::vector<PowerUp> powerUps;       // Floating power-ups
int score;                           // Current score
int lives;                           // Lives remaining
int wave;                            // Current wave
int comboCounter;                    // Consecutive kills
float comboMultiplier;               // Score multiplier
float shieldActive;                  // Shield timer
float rapidFireActive;               // Rapid fire timer
float multiShotActive;               // Multi-shot timer
float slowMotionActive;              // Slow motion timer
```

---

## 🔧 Function Breakdowns

### Function 1: loadHighScores() (Lines 40-56)

**Purpose:** Load high scores from file

**What it does:**
1. Opens "highscores.txt" file
2. Initializes all scores to {0, 0}
3. Reads up to 10 scores from file
4. Closes file safely

**Location:** Scores stored in `highscores.txt` in project root

**Modification:**
- Change filename: Replace `"highscores.txt"` with `"custom_name.txt"`
- Add encryption: Encode scores before writing
- Cloud sync: Upload to server instead of local file

---

### Function 2: saveHighScores() (Lines 58-68)

**Purpose:** Save high scores to file

**What it does:**
1. Opens "highscores.txt" for writing
2. Loops through all 10 scores
3. Writes only non-zero scores to file
4. Closes file

**File format:**
```
1000 5
950 4
850 3
...
```

---

### Function 3: insertHighScore() (Lines 70-90)

**Purpose:** Insert new score into high score list

**Algorithm:**
1. Loop through scores (top to bottom)
2. Find position where new score is larger
3. Shift all lower scores down
4. Insert new score
5. Save to file

**How to modify:**
- Change top 10 to top 20: Change `for (int i = 0; i < 10` to `< 20`
- Add name entry: Add `std::string playerName` parameter

---

### Function 4: spawnWave() (Lines 142-198)

**Purpose:** Generate a new wave of enemies

**Algorithm:**
```
1. Clear previous enemies and power-ups
2. Calculate rows: 2 + (wave / 2)
   - Wave 1-2: 2 rows
   - Wave 3-4: 3 rows
   - Wave 5+: more rows
3. Loop through rows × 6 columns
4. Determine enemy type based on wave
5. Add enemies to list
```

**Enemy type distribution by wave:**
- Wave 1-2: 70% normal, 30% weak
- Wave 3-6: 50% normal, 30% weak, 20% tank
- Wave 7+: 40% normal, 20% weak, 40% tank

**How to modify:**
- Change grid size: Modify `int cols = 6;`
- Change difficulty curve: Adjust `rows = 2 + (wave / 2)`
- Add new enemy type: Add case for type 3, 4, etc.

---

### Function 5: update() (Lines 200-380)

**Purpose:** Update game logic each frame (most complex function)

**Sub-sections:**

**5A. Enemy Movement (200-240)**
- Moves enemies left/right
- Changes direction when hitting edge
- Speeds up based on wave: `moveSpeed = 1.0f - (wave * 0.1f)`
- Game over if enemies reach bottom

**How to modify:**
- Faster movement: Change `- (wave * 0.1f)` to `- (wave * 0.15f)`
- Slower movement: Change to `- (wave * 0.05f)`
- Change direction distance: Modify `e.y += 20` (currently 20 pixels down)

**5B. Player Bullet Updates (242-290)**
- Move bullets up: `b.y -= 5.0f`
- Check collision with enemies
- Calculate points (10/20/50)
- Reduce enemy health
- Spawn power-ups (20% chance)

**How to modify:**
- Bullet speed: Change `b.y -= 5.0f` to desired speed
- Point values: Modify `pointsValue = 10/20/50`
- Power-up spawn chance: Change `rand() % 100 < 20` to different percentage

**5C. Power-Up Updates (292-325)**
- Move power-ups down: `p.y += 1.0f`
- Check collision with player
- Apply effects based on type

**How to modify:**
- Power-up fall speed: Change `p.y += 1.0f`
- Duration: Modify timer assignments (8.0f, 10.0f, 5.0f)
- Point bonus: Change `score += 100`

**5D. Enemy Bullet Updates (327-350)**
- Move down: `b.y += 2.5f`
- Check collision with player
- Shield absorption or damage

---

### Function 6: handleInput() (Lines 382-440)

**Purpose:** Handle keyboard input

**Controls:**
- Arrow Left / A: Move left
- Arrow Right / D: Move right
- Space: Shoot
- P: Pause

**Movement speed:** `playerX -= 7.0f` (or `+= 7.0f`)

**Shooting:**
- Single shot: 0.2s cooldown
- Rapid fire active: 0.1s cooldown (2x fire rate)
- Multi-shot: 3 bullets (center, left, right)

**How to modify:**
- Player movement speed: Change `7.0f` to different value
- Fire rate: Change `0.2f` cooldown value
- Add new key: Add `glfwGetKey(window, GLFW_KEY_X)` check

---

### Function 7: render() (Lines 442-580)

**Purpose:** Draw all game objects

**What's drawn:**
1. Player (green triangle)
   - With cyan shield outline if active
2. Enemies (colored squares)
   - Weak: Yellow
   - Normal: Red
   - Tank: Dark red with white outline
3. Power-ups (colored squares with outline)
   - Shield: Cyan
   - Rapid Fire: Orange
   - Multi-shot: Magenta
   - Slow Motion: Purple
4. Bullets (small rectangles)
   - Player: Yellow
   - Enemy: Orange

**How to modify:**
- Change player color: Modify `glColor3f(0.0f, 1.0f, 0.0f)` to RGB values
- Change enemy colors: Modify corresponding `glColor3f()` calls
- Change player shape: Modify triangle vertices

---

## 🎮 Feature Breakdowns

### Feature 1: Wave System

**Current Implementation:**
```cpp
// Lines 142-198: spawnWave()
int rows = 2 + (wave / 2);  // Rows increase with waves
```

**Difficulty progression:**
- Wave increases every time all enemies defeated
- More enemies per wave (rows increase)
- Better enemy mix (more tanks, fewer weak)
- Enemies move faster (moveSpeed decreases)

**How to modify:**
- Change progression rate: Modify `wave / 2` calculation
- Add boss waves: Check `if (wave % 5 == 0)` and spawn special enemy
- Cap difficulty: Add `if (moveSpeed < 0.2f) moveSpeed = 0.2f`

---

### Feature 2: Combo Multiplier System

**Current Implementation:**
```cpp
// Lines 266-273
if (comboCounter >= 20) comboMultiplier = 2.0f;
else if (comboCounter >= 10) comboMultiplier = 1.5f;
else if (comboCounter >= 5) comboMultiplier = 1.25f;
else comboMultiplier = 1.0f;
```

**How it works:**
- Resets to 0 when hit by enemy bullet
- Increases with each kill
- Affects score calculation: `score += (int)(pointsValue * wave * comboMultiplier)`

**Multiplier tiers:**
- 5+ kills: 1.25x
- 10+ kills: 1.5x
- 20+ kills: 2.0x

**How to modify:**
- Change multiplier: Modify `2.0f, 1.5f, 1.25f` values
- Add more tiers: Add `else if (comboCounter >= 30) comboMultiplier = 2.5f;`
- Change thresholds: Modify `>= 20, >= 10, >= 5`

---

### Feature 3: Power-Up System

**Four power-up types:**

| Type | Effect | Timer | Color |
|------|--------|-------|-------|
| 0 (Shield) | Blocks 1 hit | 1 unit (consumed) | Cyan |
| 1 (Rapid Fire) | 2x fire rate | 8 seconds | Orange |
| 2 (Multi-shot) | 3 bullets | 10 seconds | Magenta |
| 3 (Slow Motion) | Enemies 50% slower | 5 seconds | Purple |

**Spawn logic:**
- 20% chance when enemy dies
- Random type selected
- Falls down from enemy position

**Implementation (Lines 313-325):**
```cpp
switch (p.type) {
    case 0: shieldActive = 1.0f; break;
    case 1: rapidFireActive = 8.0f; break;
    case 2: multiShotActive = 10.0f; break;
    case 3: slowMotionActive = 5.0f; break;
}
```

**How to modify:**
- Add new power-up type: Add `case 4:` with custom effect
- Change duration: Modify timer values (8.0f, 10.0f, 5.0f)
- Change spawn chance: Modify `rand() % 100 < 20` (currently 20%)
- Add stacking: Allow multiple shield units instead of 1.0f

---

### Feature 4: Enemy AI

**Weak Enemy (Type 0):**
- Health: 1 hit
- Shoot chance: 50% of base
- Color: Yellow

**Normal Enemy (Type 1):**
- Health: 1 hit
- Shoot chance: Base rate
- Color: Red

**Tank Enemy (Type 2):**
- Health: 3 hits
- Shoot chance: 200% of base (2x)
- Color: Dark red + white outline

**Shooting frequency:**
```cpp
// Lines 359-367
int shootChance = 5 + wave * 2;  // Base: 5 + (wave × 2)
if (e.type == 2) shootChance *= 2;  // Tanks: 2x
else if (e.type == 0) shootChance /= 2;  // Weak: 0.5x
```

**How to modify:**
- Change shoot frequency: Modify `5 + wave * 2`
- Add enemy type: Create type 3, 4, 5 with custom behavior
- Change health: Modify `e.health = 3` for tanks

---

### Feature 5: Scoring System

**Score Calculation:**
```cpp
score += (int)(pointsValue * wave * comboMultiplier);
```

**Point values by enemy type:**
- Weak: 10 points
- Normal: 20 points
- Tank: 50 points

**Multipliers:**
- Wave: 1-10+
- Combo: 1.0x - 2.0x
- Power-up collection: +100 bonus

**Total example:**
- Kill Tank on Wave 5 with 15 kill combo
- Points: 50 × 5 × 1.5 = 375 points

**How to modify:**
- Change base points: Modify `pointsValue = 10/20/50`
- Change wave multiplier: Modify `* wave` to `* (wave * 0.5f)`
- Add difficulty bonus: `* (1.0f + gameSpeed)`

---

## 🎨 Customization Guide

### Customization 1: Colors

**Where colors are defined:**

**Player (Line 466):**
```cpp
glColor3f(0.0f, 1.0f, 0.0f);  // Green
// Change to: glColor3f(1.0f, 0.0f, 1.0f);  // Magenta
```

**Enemies (Lines 478-491):**
```cpp
if (e.type == 0) {
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow (Weak)
} else if (e.type == 1) {
    glColor3f(1.0f, 0.0f, 0.0f);  // Red (Normal)
} else {
    glColor3f(0.8f, 0.0f, 0.0f);  // Dark red (Tank)
}
```

**Power-ups (Lines 509-515):**
```cpp
case 0: r = 0.3f; g = 0.8f; b = 1.0f; break;  // Shield: Cyan
case 1: r = 1.0f; g = 0.8f; b = 0.0f; break;  // Rapid Fire: Orange
case 2: r = 1.0f; g = 0.0f; b = 1.0f; break;  // Multi-shot: Magenta
case 3: r = 0.5f; g = 0.0f; b = 1.0f; break;  // Slow Motion: Purple
```

**Player bullets (Line 530):**
```cpp
glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
```

**Enemy bullets (Line 542):**
```cpp
glColor3f(1.0f, 0.5f, 0.0f);  // Orange
```

**RGB Color Reference:**
```
Red:     (1.0f, 0.0f, 0.0f)
Green:   (0.0f, 1.0f, 0.0f)
Blue:    (0.0f, 0.0f, 1.0f)
Yellow:  (1.0f, 1.0f, 0.0f)
Cyan:    (0.0f, 1.0f, 1.0f)
Magenta: (1.0f, 0.0f, 1.0f)
White:   (1.0f, 1.0f, 1.0f)
Black:   (0.0f, 0.0f, 0.0f)
Orange:  (1.0f, 0.5f, 0.0f)
Purple:  (0.5f, 0.0f, 1.0f)
Gray:    (0.5f, 0.5f, 0.5f)
```

---

### Customization 2: Sizes & Dimensions

**Player size (Lines 466-475):**
```cpp
glVertex2f(playerX, playerY - 25);      // Top (change -25)
glVertex2f(playerX - 20, playerY + 20); // Left (change -20, +20)
glVertex2f(playerX + 20, playerY + 20); // Right (change +20, +20)
```
**How to modify:** Increase/decrease numbers (bigger = larger triangle)

**Enemy size (Lines 497-502):**
```cpp
glVertex2f(e.x - 15, e.y - 15);  // Change 15 to size
glVertex2f(e.x + 15, e.y - 15);
glVertex2f(e.x + 15, e.y + 15);
glVertex2f(e.x - 15, e.y + 15);
```
**Change 15 to make enemies bigger/smaller**

**Power-up size (Lines 517-522):**
```cpp
glVertex2f(p.x - 8, p.y - 8);  // Change 8 to size
```

**Bullet sizes:**
- Player bullets (Lines 532-536): Width `2`, Height `8`
- Enemy bullets (Lines 544-548): Width `2`, Height `8`

---

### Customization 3: Movement Speed

**Player movement (Line 401):**
```cpp
playerX -= 7.0f;  // Change 7.0f to desired speed
playerX += 7.0f;  // (higher = faster)
```

**Player bullet speed (Line 243):**
```cpp
b.y -= 5.0f;  // Player bullets (negative = up, higher = faster)
```

**Enemy bullet speed (Line 328):**
```cpp
b.y += 2.5f;  // Enemy bullets (positive = down, higher = faster)
```

**Power-up fall speed (Line 299):**
```cpp
p.y += 1.0f;  // (higher = faster fall)
```

**Enemy movement speed (Line 207):**
```cpp
float moveSpeed = 1.0f - (wave * 0.1f);  // Changes per wave
// Modify: 1.0f - (wave * 0.05f) = slower progression
// Modify: 1.0f - (wave * 0.15f) = faster progression
```

**Enemy horizontal move distance (Line 211):**
```cpp
e.x += 15 * enemyDirection;  // Change 15 to move distance
```

---

### Customization 4: Game Balance

**Firing rate (Lines 393-394):**
```cpp
float cooldown = 0.2f;  // Normal: 0.2 seconds between shots
if (rapidFireActive > 0) cooldown = 0.1f;  // Rapid fire: 0.1 seconds
```
**Lower = faster fire**

**Enemy shoot interval (Line 354):**
```cpp
float shootInterval = (0.5f / wave) / (slowMotionActive > 0 ? 2.0f : 1.0f);
```
**Modify `0.5f` to change frequency (lower = more shooting)**

**Combo thresholds (Lines 266-273):**
```cpp
if (comboCounter >= 20) comboMultiplier = 2.0f;    // Change 20
else if (comboCounter >= 10) comboMultiplier = 1.5f;  // Change 10
else if (comboCounter >= 5) comboMultiplier = 1.25f;  // Change 5
```

**Power-up spawn chance (Line 283):**
```cpp
if (rand() % 100 < 20) {  // 20% chance (change 20)
```

**Multi-shot spread (Lines 412-426):**
```cpp
b2.x = playerX - 15;  // Left bullet distance (change 15)
b3.x = playerX + 15;  // Right bullet distance (change 15)
```

---

### Customization 5: Game Window

**Window size (Line 596):**
```cpp
window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
// Change 640 = width, 480 = height
```

**Change window title (Line 596):**
```cpp
"Space Invaders"  // Change to desired name
```

**Game bounds (Lines 402-404):**
```cpp
if (playerX < 20) playerX = 20;    // Left boundary
if (playerX > 620) playerX = 620;  // Right boundary
// Calculated as: screenWidth - playerSize - buffer
```

---

## ⏱️ Speed & Timing Guide

### How Speed Works in the Game

**Frame timing (Line 685):**
```cpp
const double targetFrameTime = 1.0 / 60.0;  // 60 FPS (0.0167 seconds per frame)
```

**To change to 30 FPS:**
```cpp
const double targetFrameTime = 1.0 / 30.0;  // 30 FPS (slower)
```

**To change to 120 FPS:**
```cpp
const double targetFrameTime = 1.0 / 120.0;  // 120 FPS (faster)
```

### Speed Progression Table

| Parameter | Value | Effect |
|-----------|-------|--------|
| Player movement | 7.0f pixels/frame | Speed to move left/right |
| Player bullet | 5.0f pixels/frame | Speed going up |
| Enemy bullet | 2.5f pixels/frame | Speed going down |
| Power-up | 1.0f pixels/frame | Speed falling down |
| Enemy move | 15 pixels/interval | Distance moved sideways |
| Enemy down | 20 pixels/edge | Distance moved down each row |

**Real-world examples at 60 FPS:**

- Player bullet moves 5 pixels/frame × 60 frames/sec = 300 pixels/second
- At 640 pixel width, takes 2.1 seconds to cross screen
- Player movement 7 pixels/frame × 60 = 420 pixels/second
- Can cross screen in 1.5 seconds

### Wave Speed Progression

**Enemy movement per wave:**
```cpp
float moveSpeed = 1.0f - (wave * 0.1f);
```

| Wave | moveSpeed | Speed Change |
|------|-----------|--------------|
| 1 | 0.9s | Base |
| 2 | 0.8s | 11% faster |
| 3 | 0.7s | 22% faster |
| 5 | 0.5s | 44% faster |
| 7 | 0.3s | 67% faster |
| 8+ | 0.3s | Capped (min: 0.3f) |

---

## 🔧 Advanced Modifications

### Modification 1: Add New Enemy Type

**Step 1: Define type in spawnWave() (around line 180)**
```cpp
// e.type = 0 (weak), 1 (normal), 2 (tank)
// Add: e.type = 3 (new type)
```

**Step 2: Add rendering in render() (around line 478)**
```cpp
if (e.type == 3) {
    glColor3f(0.0f, 1.0f, 1.0f);  // Cyan color
    // Draw custom shape
}
```

**Step 3: Add behavior in update() (around line 254 for collision)**
```cpp
else if (e.type == 3) {
    pointsValue = 75;  // Custom points
}
```

### Modification 2: Add Screen Boundaries Indicator

**Add in render() before final render:**
```cpp
// Draw boundary lines
glColor3f(0.5f, 0.5f, 0.5f);  // Gray
glBegin(GL_LINES);
glVertex2f(20, 0);      // Left boundary
glVertex2f(20, 480);
glVertex2f(620, 0);     // Right boundary
glVertex2f(620, 480);
glEnd();
```

### Modification 3: Change Game Background Color

**Line 622 (currently black):**
```cpp
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black
// Change to: glClearColor(0.1f, 0.1f, 0.2f, 1.0f);  // Dark blue
```

### Modification 4: Add Score Multiplier Per Wave

**Current (Line 275):**
```cpp
score += (int)(pointsValue * wave * comboMultiplier);
```

**To reduce wave multiplier effect:**
```cpp
score += (int)(pointsValue * (1.0f + wave * 0.1f) * comboMultiplier);
```

### Modification 5: Make Power-ups More Powerful

**Current multi-shot (Lines 416-426):**
```cpp
if (multiShotActive > 0) {
    // 3 bullets
}
```

**Make it 5 bullets:**
```cpp
if (multiShotActive > 0) {
    // Add two more:
    Bullet b4, b5;
    b4.x = playerX - 30; b4.y = playerY - 20;
    b5.x = playerX + 30; b5.y = playerY - 20;
    // etc...
}
```

### Modification 6: Difficulty Levels

**Add at start of main():**
```cpp
int difficulty = 1;  // 1=Easy, 2=Normal, 3=Hard

// In update(), modify wave speed:
float moveSpeed = 1.0f - (wave * 0.1f * difficulty);
```

### Modification 7: Lives System Modification

**Current: 3 lives, reset on hit**

**To increase lives:**
```cpp
GameState() : ... lives(5) ...  // Change 3 to 5
```

**To shield system instead:**
```cpp
// In enemy bullet collision (around line 340)
if (shieldActive > 0) {
    shieldActive -= 0.1f;  // Damage shield gradually
} else {
    lives--;
}
```

### Modification 8: Add Invincibility Period

**After taking damage (around line 343):**
```cpp
float invincibilityTimer = 2.0f;  // 2 seconds invincible

// In render(), check before drawing:
if (invincibilityTimer <= 0 || (int)(invincibilityTimer * 10) % 2 == 0) {
    // Draw player (flashing effect)
}
```

---

## 📈 Performance Tuning

### Optimization 1: Reduce Enemy Count

**Modify spawnWave() (around line 158):**
```cpp
int cols = 6;  // Change to 4 for fewer enemies
```

### Optimization 2: Reduce Bullet Count

**Reduce spawn chance (around line 360):**
```cpp
if (rand() % 100 < shootChance)  // Increase number to reduce shooting
```

### Optimization 3: Limit Maximum Objects

**Add to update() before spawning:**
```cpp
if (playerBullets.size() > 100) return;  // Don't spawn more bullets
if (enemies.size() > 200) return;
```

---

## 🎯 Complete Customization Example

**Make "Easy Mode":**
```cpp
// In GameState constructor:
float easyDifficulty = 0.7f;  // 70% normal difficulty

// In spawnWave(), change rows:
int rows = (2 + (wave / 2)) * easyDifficulty;

// In update(), change move speed:
float moveSpeed = (1.0f - (wave * 0.1f)) / easyDifficulty;

// Change player speed:
playerX += (7.0f / easyDifficulty);  // Move faster relative to slower enemies
```

---

## 🔗 Cross-Reference Guide

| Feature | Lines | Modify For |
|---------|-------|-----------|
| Player color | 466 | Appearance |
| Enemy colors | 478-491 | Appearance |
| Power-up colors | 509-515 | Appearance |
| Player speed | 401, 404 | Movement difficulty |
| Player size | 466-475 | Visual impact |
| Enemy size | 497-502 | Visual impact |
| Bullet speed | 243, 328 | Game difficulty |
| Spawn chance | 283 | Power-up frequency |
| Wave difficulty | 158, 207 | Game progression |
| Score multiplier | 275 | Scoring |
| Fire rate | 393-394 | Player power |
| Enemy AI | 360-367 | Combat difficulty |

---

## ✅ Modification Checklist

When making changes, test:
- [ ] Game still compiles
- [ ] No crashes at startup
- [ ] Player can move left/right
- [ ] Player can shoot
- [ ] Enemies spawn
- [ ] Collision detection works
- [ ] Score increases correctly
- [ ] Game over triggers
- [ ] Wave progression works
- [ ] Power-ups work
- [ ] High scores save/load

