#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define GL_ERROR_CASE(glerror)\
    case glerror: snprintf(error, sizeof(error), "%s", #glerror)

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

// Game structures
struct Bullet {
    float x, y;
    bool active;
};

struct Enemy {
    float x, y;
    bool active;
};

struct GameState {
    float playerX;
    float playerY;
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Enemy> enemies;
    int score;
    int lives;
    int wave;
    float enemyMoveTimer;
    float enemyDirection;
    bool gameOver;
    float gameSpeed;  // Difficulty multiplier
    
    GameState() : playerX(320), playerY(420), score(0), lives(3), wave(1),
                  enemyMoveTimer(0), enemyDirection(1.0f), gameOver(false), gameSpeed(1.0f) {
        spawnWave();
    }
    
    void spawnWave() {
        enemies.clear();
        // Increase difficulty with each wave
        int rows = 2 + (wave / 2);
        int cols = 6;
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Enemy e;
                e.x = 50 + col * 90;
                e.y = 30 + row * 50;
                e.active = true;
                enemies.push_back(e);
            }
        }
    }
    
    void update(float dt) {
        if (gameOver) return;
        
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
                        e.active = false;
                        score += (10 * wave);  // More points on higher waves
                    }
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
                    lives--;
                    if (lives <= 0) gameOver = true;
                }
            }
        }
        
        // Enemy shooting - more aggressive at higher waves
        static float shootTimer = 0;
        shootTimer += dt;
        float shootInterval = 0.5f / wave;  // Faster shooting at higher waves
        shootInterval = shootInterval < 0.1f ? 0.1f : shootInterval;
        
        if (shootTimer > shootInterval) {
            shootTimer = 0;
            for (auto& e : enemies) {
                if (e.active && (rand() % 100 < (5 + wave * 2))) {
                    Bullet b;
                    b.x = e.x;
                    b.y = e.y + 20;
                    b.active = true;
                    enemyBullets.push_back(b);
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
        
        // Clean up inactive bullets
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
    }
    
    void handleInput(GLFWwindow* window) {
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
            // Shoot - faster rate of fire
            static double lastShootTime = 0;
            double currentTime = glfwGetTime();
            if (currentTime - lastShootTime > 0.2) {  // Reduced cooldown
                lastShootTime = currentTime;
                Bullet b;
                b.x = playerX;
                b.y = playerY - 20;
                b.active = true;
                playerBullets.push_back(b);
            }
        }
    }
    
    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        // Draw player (green square)
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(playerX - 20, playerY - 20);
        glVertex2f(playerX + 20, playerY - 20);
        glVertex2f(playerX + 20, playerY + 20);
        glVertex2f(playerX - 20, playerY + 20);
        glEnd();
        
        // Draw enemies (red squares)
        glColor3f(1.0f, 0.0f, 0.0f);
        for (const auto& e : enemies) {
            if (e.active) {
                glBegin(GL_QUADS);
                glVertex2f(e.x - 15, e.y - 15);
                glVertex2f(e.x + 15, e.y - 15);
                glVertex2f(e.x + 15, e.y + 15);
                glVertex2f(e.x - 15, e.y + 15);
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
            printf("\n\nGAME OVER! Final Score: %d\n", game.score);
            break;
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}