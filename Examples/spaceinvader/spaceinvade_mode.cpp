#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "vga_graphics.h"

// Game screen layout configurations
#define SW 640
#define SH 480
#define SPRITE_SCALE 5
#define ALIEN_SIZE 40    
#define PLAYER_WIDTH 40  

#define BORDER_WIDTH 15
#define LEFT_WALL_LIMIT BORDER_WIDTH
#define RIGHT_WALL_LIMIT (SW - BORDER_WIDTH)

#define FLEET_ROWS 3
#define FLEET_COLS 6
#define CEILING_Y 75

#define MAX_LASERS 3
struct Laser {
    int x;
    int y;
    bool active;
};

#define MAX_BOMBS 4
struct Bomb {
    int x;
     int y;
    bool active;
 };

struct Enemy {
    int x;
    int y;
    bool alive;
    char color;
};

int main() {
    stdio_init_all();
      initVGA();
       clearScreen();

    // Player initial stats
    int playerX = (SW - PLAYER_WIDTH) / 2;
    int playerY = 420;
    int playerSpeed = 5;
    int oldPlayerX = playerX;
    int shields =3; 
   int bombCoolTimer =0;
    Enemy fleet[FLEET_ROWS][FLEET_COLS];
    int oldFleetX[FLEET_ROWS][FLEET_COLS];
    int oldFleetY[FLEET_ROWS][FLEET_COLS];

    int fleetDirection = 1;
    int fleetSpeed = 2;
    char tierColors[FLEET_ROWS] = {RED, ORANGE, CYAN_BRIGHT};

    // Creating the 3x6 grid alignment for aliens
    for (int r = 0; r < FLEET_ROWS; r++) {
        for (int c = 0; c < FLEET_COLS; c++) {
            fleet[r][c].x = 60 + (c * 65);
             fleet[r][c].y = 90 + (r * 50);
             fleet[r][c].alive = true;
             fleet[r][c].color = tierColors[r];
            
             oldFleetX[r][c] = fleet[r][c].x;
             oldFleetY[r][c] = fleet[r][c].y;
        }
    }

    Laser lasers[MAX_LASERS];
    for (int i = 0; i < MAX_LASERS; i++) {
         lasers[i].active = false;
    }

    Bomb bombs[MAX_BOMBS];
    int oldBombX[MAX_BOMBS];
    int oldBombY[MAX_BOMBS];
    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].active = false;
        oldBombX[i] = 0;
        oldBombY[i] = 0;
    }

    int fireCooldownTimer = 0;
    const int FIRE_COOLDOWN_DELAY = 14; 

    int score = 0;
    char textBuffer[32]; 
    bool gameOver = false;
    bool victory = false;
    bool stateScreenDrawn = false;
    while (true) {
        // IT WILL EXECUTE ONLY AT A TIME OF GAMEOVER OR VICTORY
        if (gameOver || victory) {
            if (!stateScreenDrawn) {
                fillRect(LEFT_WALL_LIMIT, 50, RIGHT_WALL_LIMIT - LEFT_WALL_LIMIT, SH - 50, BLACK);
                if (gameOver) {
                    setTextSize(4); setTextColor(RED);
                    setTextCursor(SW / 2 - 100, SH / 2 - 20); 
                    writeString((char*)"GAME OVER");
                } else if (victory) {
                    setTextSize(4); setTextColor(GREEN_BRIGHT);
                    setTextCursor(SW / 2 - 80, SH / 2 - 20); 
                    writeString((char*)"VICTORY!");
                }
                
                fillRect(0, 0, BORDER_WIDTH, SH, WHITE);
                fillRect(SW - BORDER_WIDTH, 0, BORDER_WIDTH, SH, WHITE);
                
                for (int y = 0; y < SH; y += 2) {
                     drawHLine(0, y, SW, BLACK);
                }
                stateScreenDrawn = true;
            }
            sleep_ms(30);
            continue; 
          }

        // Wipe old locations with black Rectangles
        fillRect(oldPlayerX, playerY, PLAYER_WIDTH, ALIEN_SIZE, BLACK);

        for (int i = 0; i < MAX_LASERS; i++) {
            if (lasers[i].active) fillRect(lasers[i].x, lasers[i].y, 4, 12, BLACK);
        }

        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i].active) fillRect(oldBombX[i], oldBombY[i], 6, 14, BLACK);
          }
 
        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                if (fleet[r][c].alive) {
                    fillRect(oldFleetX[r][c], oldFleetY[r][c], ALIEN_SIZE, ALIEN_SIZE, BLACK);
                }
            }
          }

        fillRect(440, 10, 160, 30, BLACK); 
        fillRect(25, 30, 200, 20, BLACK);  

        oldPlayerX = playerX;
        for (int i = 0; i < MAX_BOMBS; i++) {
            oldBombX[i] = bombs[i].x;
            oldBombY[i] = bombs[i].y;
         }
        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                oldFleetX[r][c] = fleet[r][c].x; 
                 oldFleetY[r][c] = fleet[r][c].y;
            }
        }

             //controling the player using minicom
        int input_char = getchar_timeout_us(0); 

        if (input_char != PICO_ERROR_TIMEOUT) {
             if (input_char == 'a' || input_char == 'A') {
                playerX -= 12; 
             }
        
            else if (input_char == 'd' || input_char == 'D') {
                playerX += 12;
            }
            // Space button for fire 
             else if (input_char == ' ' && fireCooldownTimer == 0) {
                for (int i = 0; i < MAX_LASERS; i++) {
                    if (!lasers[i].active) {
                        lasers[i].x = playerX + (PLAYER_WIDTH / 2) - 2;
                        lasers[i].y = playerY - 12;
                        lasers[i].active = true;
                        fireCooldownTimer = FIRE_COOLDOWN_DELAY; 
                        break;
                    }
                   }
            }
         } 


        if (playerX < LEFT_WALL_LIMIT) playerX = LEFT_WALL_LIMIT;
        if (playerX > (RIGHT_WALL_LIMIT - PLAYER_WIDTH)) playerX = RIGHT_WALL_LIMIT - PLAYER_WIDTH;
        if (fireCooldownTimer > 0) fireCooldownTimer--;
 
        for (int i = 0; i < MAX_LASERS; i++) {
            if (lasers[i].active) {
                lasers[i].y -= 8; 
                if (lasers[i].y < CEILING_Y) { 
                    fillRect(lasers[i].x, lasers[i].y, 4, 12, BLACK);  
                    lasers[i].active = false; 
                }
         }
        }
              // For controling the speed of bombs
        if (bombCoolTimer >0) {
           bombCoolTimer--;
        }
        if  (bombCoolTimer ==0 && (rand() % 30 == 0)) { 
            int randR = rand() % FLEET_ROWS;
            int randC = rand() % FLEET_COLS;
            if (fleet[randR][randC].alive) {
                for (int i = 0; i < MAX_BOMBS; i++) {
                    if (!bombs[i].active) {
                        bombs[i].x = fleet[randR][randC].x + (ALIEN_SIZE / 2) - 3;
                        bombs[i].y = fleet[randR][randC].y + ALIEN_SIZE;
                        bombs[i].active = true;
                        bombCoolTimer =25;
                        break;
                    }
                   }
             }
        }

        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i].active) {
                bombs[i].y += 5; 
                if (bombs[i].y >= playerY + ALIEN_SIZE) {
                    fillRect(bombs[i].x, bombs[i].y, 6, 14, BLACK);
                    bombs[i].active = false; 
                }
            }
        }

        // Manage fleet sideways march and dropping down transitions
        bool changeDirectionAndDrop = false;
        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                if (fleet[r][c].alive) {
                    int nextX = fleet[r][c].x + (fleetDirection * fleetSpeed);
                    if (nextX <= LEFT_WALL_LIMIT || nextX >= (RIGHT_WALL_LIMIT - ALIEN_SIZE)) {
                        changeDirectionAndDrop = true;
                    }
                  }
              }
        }

        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                if (fleet[r][c].alive) {
                    if (changeDirectionAndDrop) {
                        fleet[r][c].y += 15; 
                        if (fleet[r][c].y >= playerY - ALIEN_SIZE) gameOver = true;
                    } else {
                        fleet[r][c].x += (fleetDirection * fleetSpeed); 
                    }
                }
            }
        }
        if (changeDirectionAndDrop) fleetDirection = -fleetDirection;

        // Collision Check: Score points ONLY when laser hits an alien
        for (int i = 0; i < MAX_LASERS; i++) {
            if (!lasers[i].active) continue;
            for (int r = 0; r < FLEET_ROWS; r++) {
                for (int c = 0; c < FLEET_COLS; c++) {
                    if (fleet[r][c].alive) {
                        if (lasers[i].x >= fleet[r][c].x && lasers[i].x <= (fleet[r][c].x + ALIEN_SIZE) &&
                            lasers[i].y >= fleet[r][c].y && lasers[i].y <= (fleet[r][c].y + ALIEN_SIZE)) {
                            
                            fillRect(fleet[r][c].x, fleet[r][c].y, ALIEN_SIZE, ALIEN_SIZE, BLACK);
                            fleet[r][c].alive = false; 
                            lasers[i].active = false;   
                            score += 150; 
                        }
                    }
                }
            }
        }

        // Collision Check: Bomb hitting player ship layout
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (!bombs[i].active) continue;
            if (bombs[i].x >= playerX && bombs[i].x <= (playerX + PLAYER_WIDTH) &&
                bombs[i].y >= playerY && bombs[i].y <= (playerY + ALIEN_SIZE)) {
                
                fillRect(bombs[i].x, bombs[i].y, 6, 14, BLACK);
                bombs[i].active = false; 
                shields--; 
                
                fillRect(playerX, playerY, PLAYER_WIDTH, ALIEN_SIZE, RED); 
                if (shields <= 0) gameOver = true; 
            }
        }

        bool invadersLeft = false;
        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                if (fleet[r][c].alive) invadersLeft = true;
            }
        }
        if (!invadersLeft) victory = true;

        // Render graphics scenes
        drawSprite(playerX, playerY, player_sprite, BLUE, SPRITE_SCALE);

        for (int i = 0; i < MAX_LASERS; i++) {
            if (lasers[i].active) fillRect(lasers[i].x, lasers[i].y, 4, 12, GREEN_BRIGHT); 
        }

        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i].active) fillRect(bombs[i].x, bombs[i].y, 6, 14, ORANGE); 
        }

        for (int r = 0; r < FLEET_ROWS; r++) {
            for (int c = 0; c < FLEET_COLS; c++) {
                if (fleet[r][c].alive) {
                    drawSprite(fleet[r][c].x, fleet[r][c].y, alien_sprite, fleet[r][c].color, SPRITE_SCALE);
                }
            }
        }

        // Drawing Text HUD details
        setTextSize(2); setTextColor(WHITE);
        setTextCursor(25, 10); writeString((char*)"PILOT: PICOLINK");
        
        sprintf(textBuffer, "SCORE: %05d", score);
        setTextCursor(25, 30); writeString(textBuffer);

        sprintf(textBuffer, "SHIELDS: %d/3", shields);
        setTextCursor(440, 15);
        if (shields == 3) setTextColor(GREEN_BRIGHT);
        else if (shields == 2) setTextColor(ORANGE);
        else setTextColor(RED);
        writeString(textBuffer);

        drawHLine(LEFT_WALL_LIMIT, CEILING_Y, RIGHT_WALL_LIMIT - LEFT_WALL_LIMIT, WHITE);
        fillRect(0, 0, BORDER_WIDTH, SH, WHITE);
        fillRect(SW - BORDER_WIDTH, 0, BORDER_WIDTH, SH, WHITE);

        sleep_ms(16);
    }
}
