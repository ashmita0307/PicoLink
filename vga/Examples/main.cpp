#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "vga_graphics.h"

/* ── Screen ───────────────── */
#define SW 640
#define SH 480

#define SCALE 4

void printCentered(const char* text, int y, char color) {
    int len = strlen(text);
    int textWidth = len * 6 * SCALE;
    int x = (SW - textWidth) / 2;

    for (int i = 0; i < len; i++) {
        drawChar(x + i * 6 * SCALE, y, text[i], color, BLACK, SCALE);
    }
}

void drawScanlines() {
    for (int y = 0; y < SH; y += 2) {
        drawHLine(0, y, SW, BLACK);
    }
}

void drawColorPaletteBar() {
    int barWidth = SW / 16;
    int barHeight = 24; 
    int startY = SH - barHeight;

    for (int i = 0; i < 16; i++) {
        fillRect(i * barWidth, startY, barWidth, barHeight, (char)i);
    }
}

int main() {

    stdio_init_all();

    initVGA();
    clearScreen();

    int lineHeight = 8 * SCALE;

    int totalHeight = lineHeight * 3 + 20;
    int startY = (SH - totalHeight) / 2;

    // To Print Hello World In 3 Diffrenet colors
    printCentered("PICO LINK 1", startY, RED);
    printCentered("PICO LINK 2", startY + lineHeight + 10, ORANGE); // New mixed color shade!
    printCentered("PICO LINK 3", startY + (lineHeight + 10) * 2, BLUE);

    drawScanlines();

    drawColorPaletteBar();

    while (true) {
        tight_loop_contents();
    }
}
