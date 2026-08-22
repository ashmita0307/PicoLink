#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#include "pico/stdlib.h"
#include "vga_graphics.h"

#define CELL_SIZE 19
#define GRID_W 32
#define GRID_H 24
#define HUD_HEIGHT 24
bool gameStarted = false;

void drawHUD();
void addScore(int pts);
void drawPlayer();
void drawGhost(int gx, int gy, char color);
void eraseGhostCell(int gx, int gy);
void validateMaze();
bool isWall(int gx, int gy);
void drawMaze();
void erasePlayerCell(int gx, int gy);
void tryMove(int dx, int dy, int dir);
bool checkGhostCollision();
void redrawIfOccupied(int gx, int gy);
void respawnPlayer();
void handleCollision();
void handleInput();
void moveGhostSmart(int &gx, int &gy, int &lastDir, char color);
void resetGhosts();

char maze[GRID_H][GRID_W + 1] = {
    "################################",                
    "# ............................ #",
    "# .######## . # . #. ########. #",
    "# .... #..... # . #.... #..... #",
    "##. #. #. ######. #. #. #.  .###",
    "# . #............... #....# .. #",
    "# .##.###### . # . #### . ###. #",
    "# ...... # ... # ............. #",
    "#  . # . # .#######  .    .    #",
    "###. # ................###. ####",
    "###. #  . ############.  #. ####",
    "   ...... #          #.....     ",
    "###. ###. ####   #####.  #. ####",
    "###. #  .             .  #. ####",
    "#  . # ................ ##.    #",
    "#  .   . # .#######  .    .    #",
    "# ...... # ... # ............. #",
    "# .##.###### . # . #### . ###. #",
    "# . #............... #....# .. #",
    "##. #. #. ######. #. #. #.  .###",
    "# .... #..... # . #.... #..... #",
    "# .######## . # . #. ########. #",
    "# ............................ #",
    "################################"
};

int ghostRedDir = -1;  
int invulnerableFrames = 0;
bool gameOver = false;
int playerGX = 1, playerGY = 1;
int playerDir = 0; 
#define PLAYER_COLOR YELLOW_BRIGHT

int score = 0;
int lives = 3;
int frameCount = 0;

int playerSpawnGX = 1, playerSpawnGY = 1;

void drawHUD() {
    fillRect(0, 0, screenWidth, HUD_HEIGHT, BLACK);

    setTextSize(2);
    setTextColor2(WHITE, BLACK);
    setTextCursor(10, 4);

    char buf[40];
    snprintf(buf, sizeof(buf), "SCORE:%d  LIVES LEFT:%d", score, lives);
    writeString(buf);

    if (gameOver) {
        setTextCursor(230, 4);
        setTextColor2(RED, BLACK);
        writeString((char*)"GAME OVER");
    }
}

void addScore(int pts) {
    score += pts;
    printf("Score: %d\n", score);
    drawHUD();
}

void drawPlayer() {
    int cx = playerGX * CELL_SIZE + CELL_SIZE / 2;
    int cy = playerGY * CELL_SIZE + CELL_SIZE / 2 + HUD_HEIGHT;
    int r  = CELL_SIZE / 2 - 2;

    fillCircle(cx, cy, r, PLAYER_COLOR);

    float baseDeg;
    switch (playerDir) {
        case 1:  baseDeg = 90;  break;
        case 2:  baseDeg = 180; break;
        case 3:  baseDeg = 270; break;
        default: baseDeg = 0;   break;
    }

    float mouthHalfAngle = 35.0f; 
    for (float a = -mouthHalfAngle; a <= mouthHalfAngle; a += 2.0f) {
        float rad = (baseDeg + a) * 3.14159265f / 180.0f;
        int ex = cx + (int)(r * cosf(rad));
        int ey = cy + (int)(r * sinf(rad));
        drawLine(cx, cy, ex, ey, BLACK); 
    }
}

int ghostRedGX = 15,    ghostRedGY = 11;
int ghostPinkGX = 13,   ghostPinkGY = 11;
int ghostBlueGX = 17,   ghostBlueGY = 11;
int ghostOrangeGX = 19, ghostOrangeGY = 11;

void drawGhost(int gx, int gy, char color) {
    int cx = gx * CELL_SIZE + CELL_SIZE / 2;
    int cy = gy * CELL_SIZE + CELL_SIZE / 2 + HUD_HEIGHT;
    int r  = CELL_SIZE / 2 - 1;

    fillCircle(cx, cy - 2, r, color);

    fillRect(cx - r, cy - 2, r * 2, r + 3, color);

    int notchW = (r * 2) / 3;
    int baseY  = cy + r + 1;
    for (int i = 0; i < 3; i++) {
        int nx = cx - r + i * notchW;
        for (int k = 0; k < notchW / 2; k++) {
            drawLine(nx + k, baseY, nx + k, baseY - k, BLACK);
            drawLine(nx + notchW - k, baseY, nx + notchW - k, baseY - k, BLACK);
        }
    }

    fillCircle(cx - 4, cy - 4, 3, WHITE);
    fillCircle(cx + 4, cy - 4, 3, WHITE);
    fillCircle(cx - 3, cy - 4, 1, BLACK);
    fillCircle(cx + 5, cy - 4, 1, BLACK);
}

void eraseGhostCell(int gx, int gy) {
    int x = gx * CELL_SIZE;
    int y = gy * CELL_SIZE + HUD_HEIGHT;
    fillRect(x, y, CELL_SIZE, CELL_SIZE, BLACK);

    char c = maze[gy][gx];
    if (c == '.') {
        int cx = x + CELL_SIZE / 2;
        int cy = y + CELL_SIZE / 2;
        fillRect(cx - 2, cy - 2, 5, 5, YELLOW_BRIGHT);
    }

    redrawIfOccupied(gx, gy); 
}

void validateMaze() {
    for (int i = 0; i < GRID_H; i++) {
        if ((int)strlen(maze[i]) != GRID_W) {
            printf("MAZE ERROR: row %d is %d chars, expected %d\n",
                   i, (int)strlen(maze[i]), GRID_W);
        }
    }
}

#define WALL_THICK 4

bool isWall(int gx, int gy) {
    if (gx < 0 || gx >= GRID_W || gy < 0 || gy >= GRID_H) return false;
    return maze[gy][gx] == '#';
}

void drawMaze() {
    for (int gy = 0; gy < GRID_H; gy++) {
        for (int gx = 0; gx < GRID_W; gx++) {
            int x = gx * CELL_SIZE;
            int y = gy * CELL_SIZE + HUD_HEIGHT;
            char c = maze[gy][gx];

            if (c == '#') {
                bool up    = !isWall(gx, gy - 1);
                bool down  = !isWall(gx, gy + 1);
                bool left  = !isWall(gx - 1, gy);
                bool right = !isWall(gx + 1, gy);

                if (up)    fillRect(x, y, CELL_SIZE, WALL_THICK, BLUE);
                if (down)  fillRect(x, y + CELL_SIZE - WALL_THICK, CELL_SIZE, WALL_THICK, BLUE);
                if (left)  fillRect(x, y, WALL_THICK, CELL_SIZE, BLUE);
                if (right) fillRect(x + CELL_SIZE - WALL_THICK, y, WALL_THICK, CELL_SIZE, BLUE);
            }
            else if (c == '.') {
                int cx = x + CELL_SIZE / 2;
                int cy = y + CELL_SIZE / 2;
                fillRect(cx - 2, cy - 2, 5, 5, YELLOW_BRIGHT);
            }
        }
    }
}

void erasePlayerCell(int gx, int gy) {
    int x = gx * CELL_SIZE;
    int y = gy * CELL_SIZE + HUD_HEIGHT;
    fillRect(x, y, CELL_SIZE, CELL_SIZE, BLACK);

    char c = maze[gy][gx];
    if (c == '.') {
        int cx = x + CELL_SIZE / 2;
        int cy = y + CELL_SIZE / 2;
        fillRect(cx - 2, cy - 2, 5, 5, YELLOW_BRIGHT);
    }

    redrawIfOccupied(gx, gy);   // <-- add this
}

void tryMove(int dx, int dy, int dir) {
    int newGX = playerGX + dx;
    int newGY = playerGY + dy;

    if (!isWall(newGX, newGY)) {
        int oldGX = playerGX;
        int oldGY = playerGY;

        playerGX = newGX;
        playerGY = newGY;
        playerDir = dir;

        if (maze[playerGY][playerGX] == '.') {
            maze[playerGY][playerGX] = ' ';
        }

        erasePlayerCell(oldGX, oldGY); 
        drawPlayer();                  
    }
}

bool checkGhostCollision() {
    if ((playerGX == ghostRedGX    && playerGY == ghostRedGY)    ||
        (playerGX == ghostPinkGX   && playerGY == ghostPinkGY)   ||
        (playerGX == ghostBlueGX   && playerGY == ghostBlueGY)   ||
        (playerGX == ghostOrangeGX && playerGY == ghostOrangeGY)) {
        return true;
    }
    return false;
}

void redrawIfOccupied(int gx, int gy) {
    if (playerGX == gx && playerGY == gy) { drawPlayer(); return; }
    if (ghostRedGX == gx && ghostRedGY == gy) { drawGhost(gx, gy, RED); return; }
    if (ghostPinkGX == gx && ghostPinkGY == gy) { drawGhost(gx, gy, PINK); return; }
    if (ghostBlueGX == gx && ghostBlueGY == gy) { drawGhost(gx, gy, BLUE); return; }
    if (ghostOrangeGX == gx && ghostOrangeGY == gy) { drawGhost(gx, gy, ORANGE); return; }
}

void respawnPlayer() {
    int oldGX = playerGX, oldGY = playerGY;

    playerGX = playerSpawnGX;
    playerGY = playerSpawnGY;
    playerDir = 0;

    erasePlayerCell(oldGX, oldGY); 
    drawPlayer();
}

void handleCollision() {
    if (gameOver) return;

    if (invulnerableFrames > 0) {
        invulnerableFrames--;
        return;
    }

    if (checkGhostCollision()) {
        lives--;
        if (lives < 0) lives = 0;
        printf("Hit! Lives left: %d\n", lives);

        if (lives == 0) {
            gameOver = true;
            drawHUD();
            return;
        }

        respawnPlayer();
        resetGhosts();
        invulnerableFrames = 20;
        drawHUD();
    }
}

void handleInput() {
    int ch = getchar_timeout_us(0);
    if (ch == PICO_ERROR_TIMEOUT) return;

    switch (ch) {
        case 'w': case 'W': gameStarted = true; tryMove(0, -1, 3); break;
        case 's': case 'S': gameStarted = true; tryMove(0,  1, 1); break;
        case 'a': case 'A': gameStarted = true; tryMove(-1, 0, 2); break;
        case 'd': case 'D': gameStarted = true; tryMove(1,  0, 0); break;
    }
}

void moveGhostSmart(int &gx, int &gy, int &lastDir, char color) {
    int oldGX = gx, oldGY = gy;
    int dxs[4] = {1, 0, -1, 0};
    int dys[4] = {0, 1, 0, -1};
    int opposite[4] = {2, 3, 0, 1};

    int bestDir = -1;
    long bestDist = -1;

    for (int d = 0; d < 4; d++) {
        if (lastDir != -1 && d == opposite[lastDir]) continue; 
        int nx = gx + dxs[d];
        int ny = gy + dys[d];
        if (isWall(nx, ny)) continue;
        long dist = (long)(nx - playerGX) * (nx - playerGX) + (long)(ny - playerGY) * (ny - playerGY);
        if (bestDist == -1 || dist < bestDist) { bestDist = dist; bestDir = d; }
    }

    if (bestDir == -1) { 
        for (int d = 0; d < 4; d++) {
            int nx = gx + dxs[d];
            int ny = gy + dys[d];
            if (isWall(nx, ny)) continue;
            long dist = (long)(nx - playerGX) * (nx - playerGX) + (long)(ny - playerGY) * (ny - playerGY);
            if (bestDist == -1 || dist < bestDist) { bestDist = dist; bestDir = d; }
        }
    }

    if (bestDir != -1) {
        gx += dxs[bestDir];
        gy += dys[bestDir];
        lastDir = bestDir;
        eraseGhostCell(oldGX, oldGY);
        drawGhost(gx, gy, color);
    }
}

void resetGhosts() {
    int oldRedGX = ghostRedGX, oldRedGY = ghostRedGY;

    ghostRedGX = 15; ghostRedGY = 11;
    ghostPinkGX = 13; ghostPinkGY = 11;
    ghostBlueGX = 17; ghostBlueGY = 11;
    ghostOrangeGX = 19; ghostOrangeGY = 11;
    ghostRedDir = -1;

    eraseGhostCell(oldRedGX, oldRedGY); 

    drawGhost(ghostRedGX, ghostRedGY, RED);
    drawGhost(ghostPinkGX, ghostPinkGY, PINK);
    drawGhost(ghostBlueGX, ghostBlueGY, BLUE);
    drawGhost(ghostOrangeGX, ghostOrangeGY, ORANGE);
}

int main() {
    stdio_init_all();
    initVGA();
    validateMaze();
    clearScreen();

    drawMaze();
    drawHUD();
    drawGhost(ghostRedGX,    ghostRedGY,    RED);
    drawGhost(ghostPinkGX,   ghostPinkGY,   PINK);
    drawGhost(ghostBlueGX,   ghostBlueGY,   BLUE);
    drawGhost(ghostOrangeGX, ghostOrangeGY, ORANGE);
    drawPlayer();

   while (true) {
    if (!gameOver) {
        handleInput();

        frameCount++;
        if (gameStarted && frameCount % 6 == 0) {
            moveGhostSmart(ghostRedGX, ghostRedGY, ghostRedDir, RED);
            handleCollision();
        }
    }
    sleep_ms(50);
}
    return 0;
}