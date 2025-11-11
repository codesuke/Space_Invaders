#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <array>

#define GL_ERROR_CASE(glerror)\
    case glerror: snprintf(error, sizeof(error), "%s", #glerror)

// Game structures (forward declarations)
struct HighScore {
    int score;
    int wave;
};

inline void gl_debug(const char *file, int line) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        char error[128];

        switch(err) {
            GL_ERROR_CASE(GL_INVALID_ENUM); break;
            GL_ERROR_CASE(GL_INVALID_VALUE); break;
            GL_ERROR_CASE(GL_INVALID_OPERATION); break;
            GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION); break;
            GL_ERROR_CASE(GL_OUT_OF_MEMORY); break;
            default: snprintf(error, sizeof(error), "%s", "UNKNOWN_ERROR"); break;
        }

        fprintf(stderr, "%s - %s: %d\n", error, file, line);
    }
}

// High score file management
void loadHighScores(std::array<HighScore, 10>& scores) {
    std::ifstream file("highscores.txt");
    
    // Initialize with zeros
    for (int i = 0; i < 10; i++) {
        scores[i] = {0, 0};
    }
    
    if (file.is_open()) {
        for (int i = 0; i < 10; i++) {
            if (file >> scores[i].score >> scores[i].wave) {
                // Successfully read
            } else {
                break;
            }
        }
        file.close();
    }
}

void saveHighScores(const std::array<HighScore, 10>& scores) {
    std::ofstream file("highscores.txt");
    if (file.is_open()) {
        for (int i = 0; i < 10; i++) {
            if (scores[i].score > 0) {
                file << scores[i].score << " " << scores[i].wave << "\n";
            }
        }
        file.close();
    }
}

void insertHighScore(std::array<HighScore, 10>& scores, int newScore, int newWave) {
    // Find the position to insert
    for (int i = 0; i < 10; i++) {
        if (newScore > scores[i].score) {
            // Shift scores down
            for (int j = 9; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            scores[i] = {newScore, newWave};
            saveHighScores(scores);
            return;
        }
    }
}

// Game structures
struct Bullet {
    float x, y;
    bool active;
};

struct PowerUp {
    float x, y;
    int type;  // 0=shield, 1=rapidfire, 2=multishot, 3=slowmotion
    bool active;
};

struct Enemy {
    float x, y;
    int type;     // 0=weak, 1=normal, 2=tank
    int health;   // Number of hits to destroy
    bool active;
};

struct GameState {
    float playerX;
    float playerY;
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Enemy> enemies;
    std::vector<PowerUp> powerUps;
    int score;
    int lives;
    int wave;
    int comboCounter;
    float comboMultiplier;
    float enemyMoveTimer;
    float enemyDirection;
    bool gameOver;
    bool paused;
    float gameSpeed;  // Difficulty multiplier
    std::array<HighScore, 10> highScores;
    int nextHighScoreIndex;
    
    // Power-up timers
    float shieldActive;      // 0 = inactive
    float rapidFireActive;   // 0 = inactive
    float multiShotActive;   // 0 = inactive
    float slowMotionActive;  // 0 = inactive
    
    GameState() : playerX(320), playerY(420), score(0), lives(3), wave(1), comboCounter(0), comboMultiplier(1.0f),
                  enemyMoveTimer(0), enemyDirection(1.0f), gameOver(false), paused(false), gameSpeed(1.0f), nextHighScoreIndex(0),
                  shieldActive(0), rapidFireActive(0), multiShotActive(0), slowMotionActive(0) {
        loadHighScores(highScores);
        spawnWave();
    }
    
    void spawnWave() {
        enemies.clear();
        powerUps.clear();
        
        // Increase difficulty with each wave
        int rows = 2 + (wave / 2);
        int cols = 6;
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Enemy e;
                e.x = 50 + col * 90;
                e.y = 30 + row * 50;
                e.active = true;
                
                // Determine enemy type based on wave
                int rand_type = rand() % 100;
                if (wave < 3) {
                    // Early waves: mostly normal enemies
                    e.type = (rand_type < 70) ? 1 : 0;  // 70% normal, 30% weak
                } else if (wave < 7) {
                    // Mid waves: mix of all types
                    if (rand_type < 50) e.type = 1;      // 50% normal
                    else if (rand_type < 80) e.type = 0; // 30% weak
                    else e.type = 2;                      // 20% tank
                } else {
                    // Later waves: more tanks
                    if (rand_type < 40) e.type = 1;      // 40% normal
                    else if (rand_type < 60) e.type = 0; // 20% weak
                    else e.type = 2;                      // 40% tank
                }
                
                // Set health based on type
                if (e.type == 0) e.health = 1;      // Weak: 1 hit
                else if (e.type == 1) e.health = 1; // Normal: 1 hit
                else e.health = 3;                  // Tank: 3 hits
                
                enemies.push_back(e);
            }
        }
    }
    
    void update(float dt) {
        if (gameOver || paused) return;
        
        // Move enemies with difficulty scaling
        enemyMoveTimer += dt;
        float moveSpeed = 1.0f - (wave * 0.1f);  // Gets faster with each wave
        moveSpeed = moveSpeed < 0.3f ? 0.3f : moveSpeed;
        
        if (enemyMoveTimer > moveSpeed) {
            enemyMoveTimer = 0;
            bool hitEdge = false;
            
            for (auto& e : enemies) {
                if (e.active) {
                    e.x += 15 * enemyDirection;
                    if (e.x < 20 || e.x > 600) {
                        hitEdge = true;
                    }
                }
            }
            
            if (hitEdge) {
                enemyDirection *= -1.0f;
                for (auto& e : enemies) {
                    if (e.active) {
                        e.y += 20;
                        if (e.y > 400) {
                            gameOver = true;
                        }
                    }
                }
            }
        }
        
        // Update bullets
        for (auto& b : playerBullets) {
            if (b.active) {
                b.y -= 5.0f;  // Slightly faster bullets
                if (b.y < 0) b.active = false;
                
                // Check collision with enemies
                for (auto& e : enemies) {
                    if (e.active && 
                        b.x > e.x - 15 && b.x < e.x + 15 &&
                        b.y > e.y - 15 && b.y < e.y + 15) {
                        b.active = false;
                        
                        // Reduce health based on enemy type
                        int pointsValue = 0;
                        if (e.type == 0) pointsValue = 10;      // Weak enemy: 10 points
                        else if (e.type == 1) pointsValue = 20; // Normal enemy: 20 points
                        else pointsValue = 50;                  // Tank enemy: 50 points
                        
                        e.health--;
                        
                        // Check if enemy defeated
                        if (e.health <= 0) {
                            e.active = false;
                            comboCounter++;
                            
                            // Update combo multiplier
                            if (comboCounter >= 20) comboMultiplier = 2.0f;
                            else if (comboCounter >= 10) comboMultiplier = 1.5f;
                            else if (comboCounter >= 5) comboMultiplier = 1.25f;
                            else comboMultiplier = 1.0f;
                            
                            // Calculate score with combo multiplier
                            score += (int)(pointsValue * wave * comboMultiplier);
                            
                            // Spawn power-up (20% chance)
                            if (rand() % 100 < 20) {
                                PowerUp p;
                                p.x = e.x;
                                p.y = e.y;
                                p.type = rand() % 4;  // Random power-up type
                                p.active = true;
                                powerUps.push_back(p);
                            }
                        }
                    }
                }
            }
        }
        
        // Update power-ups
        for (auto& p : powerUps) {
            if (p.active) {
                // Power-ups float down slowly
                p.y += 1.0f;
                if (p.y > 480) p.active = false;
                
                // Check collision with player
                if (p.x > playerX - 20 && p.x < playerX + 20 &&
                    p.y > playerY - 25 && p.y < playerY + 20) {
                    p.active = false;
                    
                    // Apply power-up based on type
                    switch (p.type) {
                        case 0: shieldActive = 1.0f; break;        // Shield: 1 unit
                        case 1: rapidFireActive = 8.0f; break;     // Rapid fire: 8 seconds
                        case 2: multiShotActive = 10.0f; break;    // Multi-shot: 10 seconds
                        case 3: slowMotionActive = 5.0f; break;    // Slow motion: 5 seconds
                    }
                    
                    score += 100;  // Bonus for collecting power-up
                }
            }
        }
        
        for (auto& b : enemyBullets) {
            if (b.active) {
                b.y += 2.5f;
                if (b.y > 480) b.active = false;
                
                // Check collision with player
                if (b.x > playerX - 20 && b.x < playerX + 20 &&
                    b.y > playerY - 20 && b.y < playerY + 20) {
                    b.active = false;
                    
                    // Check if shield is active
                    if (shieldActive > 0) {
                        shieldActive = 0;  // Shield blocks one hit
                    } else {
                        comboCounter = 0;  // Reset combo on hit
                        comboMultiplier = 1.0f;
                        lives--;
                        if (lives <= 0) gameOver = true;
                    }
                }
            }
        }
        
        // Update power-up timers
        if (shieldActive > 0) shieldActive -= dt;
        if (rapidFireActive > 0) rapidFireActive -= dt;
        if (multiShotActive > 0) multiShotActive -= dt;
        if (slowMotionActive > 0) slowMotionActive -= dt;
        
        // Enemy shooting - more aggressive at higher waves
        static float shootTimer = 0;
        shootTimer += dt;
        
        // Adjust shoot interval based on slow motion
        float shootInterval = (0.5f / wave) / (slowMotionActive > 0 ? 2.0f : 1.0f);
        shootInterval = shootInterval < 0.1f ? 0.1f : shootInterval;
        
        if (shootTimer > shootInterval) {
            shootTimer = 0;
            for (auto& e : enemies) {
                if (e.active) {
                    // Tank enemies shoot more frequently
                    int shootChance = 5 + wave * 2;
                    if (e.type == 2) shootChance *= 2;  // Tank: 2x more often
                    else if (e.type == 0) shootChance /= 2;  // Weak: half as often
                    
                    if (rand() % 100 < shootChance) {
                        Bullet b;
                        b.x = e.x;
                        b.y = e.y + 20;
                        b.active = true;
                        enemyBullets.push_back(b);
                    }
                }
            }
        }
        
        // Check if all enemies defeated
        bool allDefeated = true;
        for (auto& e : enemies) {
            if (e.active) {
                allDefeated = false;
                break;
            }
        }
        
        if (allDefeated) {
            // Next wave!
            wave++;
            spawnWave();
        }
        
        // Clean up inactive bullets and power-ups
        playerBullets.erase(
            std::remove_if(playerBullets.begin(), playerBullets.end(),
                          [](const Bullet& b) { return !b.active; }),
            playerBullets.end()
        );
        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                          [](const Bullet& b) { return !b.active; }),
            enemyBullets.end()
        );
        powerUps.erase(
            std::remove_if(powerUps.begin(), powerUps.end(),
                          [](const PowerUp& p) { return !p.active; }),
            powerUps.end()
        );
    }
    
    void handleInput(GLFWwindow* window) {
        // Toggle pause
        static bool pKeyPressed = false;
        bool pKeyDown = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
        if (pKeyDown && !pKeyPressed) {
            paused = !paused;
        }
        pKeyPressed = pKeyDown;
        
        if (paused) return;  // Don't process other input while paused
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || 
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            playerX -= 7.0f;  // Slightly faster movement
            if (playerX < 20) playerX = 20;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || 
            glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            playerX += 7.0f;
            if (playerX > 620) playerX = 620;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            // Shoot - with power-up support
            static double lastShootTime = 0;
            double currentTime = glfwGetTime();
            
            // Adjust cooldown based on rapid fire power-up
            float cooldown = 0.2f;
            if (rapidFireActive > 0) cooldown = 0.1f;  // 2x fire rate
            
            if (currentTime - lastShootTime > cooldown) {
                lastShootTime = currentTime;
                
                // Multi-shot mode: 3 bullets
                if (multiShotActive > 0) {
                    Bullet b1, b2, b3;
                    
                    // Center bullet
                    b1.x = playerX;
                    b1.y = playerY - 20;
                    b1.active = true;
                    playerBullets.push_back(b1);
                    
                    // Left bullet
                    b2.x = playerX - 15;
                    b2.y = playerY - 20;
                    b2.active = true;
                    playerBullets.push_back(b2);
                    
                    // Right bullet
                    b3.x = playerX + 15;
                    b3.y = playerY - 20;
                    b3.active = true;
                    playerBullets.push_back(b3);
                } else {
                    // Normal single shot
                    Bullet b;
                    b.x = playerX;
                    b.y = playerY - 20;
                    b.active = true;
                    playerBullets.push_back(b);
                }
            }
        }
    }
    
    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        // Draw player (green triangle - spaceship shape)
        // If shield is active, draw an outline
        if (shieldActive > 0) {
            glColor3f(0.3f, 0.8f, 1.0f);  // Cyan outline
            glBegin(GL_LINE_LOOP);
            glVertex2f(playerX, playerY - 30);
            glVertex2f(playerX - 25, playerY + 25);
            glVertex2f(playerX + 25, playerY + 25);
            glEnd();
        }
        
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(playerX, playerY - 25);      // Top point
        glVertex2f(playerX - 20, playerY + 20); // Bottom left
        glVertex2f(playerX + 20, playerY + 20); // Bottom right
        glEnd();
        
        // Draw enemies with different colors based on type
        for (const auto& e : enemies) {
            if (e.active) {
                // Color based on enemy type
                if (e.type == 0) {
                    // Weak: Yellow
                    glColor3f(1.0f, 1.0f, 0.0f);
                } else if (e.type == 1) {
                    // Normal: Red
                    glColor3f(1.0f, 0.0f, 0.0f);
                } else {
                    // Tank: Dark Red/Maroon
                    glColor3f(0.8f, 0.0f, 0.0f);
                }
                
                // Draw main body
                glBegin(GL_QUADS);
                glVertex2f(e.x - 15, e.y - 15);
                glVertex2f(e.x + 15, e.y - 15);
                glVertex2f(e.x + 15, e.y + 15);
                glVertex2f(e.x - 15, e.y + 15);
                glEnd();
                
                // Draw health indicator for tanks
                if (e.type == 2 && e.health > 0) {
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glBegin(GL_LINE_LOOP);
                    glVertex2f(e.x - 18, e.y - 18);
                    glVertex2f(e.x + 18, e.y - 18);
                    glVertex2f(e.x + 18, e.y + 18);
                    glVertex2f(e.x - 18, e.y + 18);
                    glEnd();
                }
            }
        }
        
        // Draw power-ups with different colors and glowing effect
        for (const auto& p : powerUps) {
            if (p.active) {
                float r, g, b;
                switch (p.type) {
                    case 0: r = 0.3f; g = 0.8f; b = 1.0f; break;  // Shield: Cyan
                    case 1: r = 1.0f; g = 0.8f; b = 0.0f; break;  // Rapid Fire: Orange
                    case 2: r = 1.0f; g = 0.0f; b = 1.0f; break;  // Multi-shot: Magenta
                    case 3: r = 0.5f; g = 0.0f; b = 1.0f; break;  // Slow Motion: Purple
                    default: r = 1.0f; g = 1.0f; b = 1.0f;
                }
                
                glColor3f(r, g, b);
                glBegin(GL_QUADS);
                glVertex2f(p.x - 8, p.y - 8);
                glVertex2f(p.x + 8, p.y - 8);
                glVertex2f(p.x + 8, p.y + 8);
                glVertex2f(p.x - 8, p.y + 8);
                glEnd();
                
                // Draw glowing outline
                glColor3f(r * 1.5f, g * 1.5f, b * 1.5f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(p.x - 12, p.y - 12);
                glVertex2f(p.x + 12, p.y - 12);
                glVertex2f(p.x + 12, p.y + 12);
                glVertex2f(p.x - 12, p.y + 12);
                glEnd();
            }
        }
        
        // Draw player bullets (yellow)
        glColor3f(1.0f, 1.0f, 0.0f);
        for (const auto& b : playerBullets) {
            if (b.active) {
                glBegin(GL_QUADS);
                glVertex2f(b.x - 2, b.y - 8);
                glVertex2f(b.x + 2, b.y - 8);
                glVertex2f(b.x + 2, b.y + 8);
                glVertex2f(b.x - 2, b.y + 8);
                glEnd();
            }
        }
        
        // Draw enemy bullets (orange)
        glColor3f(1.0f, 0.5f, 0.0f);
        for (const auto& b : enemyBullets) {
            if (b.active) {
                glBegin(GL_QUADS);
                glVertex2f(b.x - 2, b.y - 8);
                glVertex2f(b.x + 2, b.y - 8);
                glVertex2f(b.x + 2, b.y + 8);
                glVertex2f(b.x - 2, b.y + 8);
                glEnd();
            }
        }
        
        // Draw pause indicator
        if (paused) {
            glColor3f(1.0f, 1.0f, 1.0f);
            // Draw simple text representation: display via console
            printf(" [PAUSED]");
        }
    }
};

int main(int argc, char* argv[])
{
    srand((unsigned int)time(0));

    GLFWwindow* window;

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        fprintf(stderr, "Error initializing GLEW.\n");
        glfwTerminate();
        return -1;
    }
    int glVersion[2] = {-1, 1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    gl_debug(__FILE__, __LINE__);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Setup OpenGL state
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 640, 480, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create game state
    GameState game;
    
    // Set up timing for consistent frame rate (60 FPS)
    double lastTime = glfwGetTime();
    const double targetFrameTime = 1.0 / 60.0; // 60 FPS
    
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        
        // Limit frame rate to 60 FPS
        if (deltaTime < targetFrameTime) {
            continue;
        }
        
        lastTime = currentTime;
        
        // Handle input and update game
        game.handleInput(window);
        game.update((float)deltaTime);
        game.render();
        
        // Print stats
        printf("\rWave: %d | Score: %d | Lives: %d | Enemies: %d", game.wave, game.score, game.lives, 
               (int)std::count_if(game.enemies.begin(), game.enemies.end(), 
                            [](const Enemy& e) { return e.active; }));
        fflush(stdout);
        
        if (game.gameOver) {
            printf("\n\n========== GAME OVER ==========\n");
            printf("Final Score: %d\n", game.score);
            printf("Wave Reached: %d\n", game.wave);
            
            // Check if high score
            bool isHighScore = false;
            for (int i = 0; i < 10; i++) {
                if (game.score > game.highScores[i].score) {
                    isHighScore = true;
                    break;
                }
            }
            
            if (isHighScore) {
                printf("\n*** NEW HIGH SCORE! ***\n");
                insertHighScore(game.highScores, game.score, game.wave);
            }
            
            printf("\n===== TOP 10 HIGH SCORES =====\n");
            for (int i = 0; i < 10; i++) {
                if (game.highScores[i].score > 0) {
                    printf("%d. %d (Wave %d)\n", i + 1, game.highScores[i].score, game.highScores[i].wave);
                }
            }
            printf("\nPress any key to exit...\n");
            
            break;
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}