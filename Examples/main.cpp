#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "vga_graphics.h"

/* ── Screen ───────────────── */
#define SW 640
#define SH 480

/* ── Text scale ──────────── */
#define SCALE 4

/* ── Centered print ───────── */
void printCentered(const char* text, int y, char color) {
    int len = strlen(text);
    int textWidth = len * 6 * SCALE;
    int x = (SW - textWidth) / 2;

    for (int i = 0; i < len; i++) {
        drawChar(x + i * 6 * SCALE, y, text[i], color, BLACK, SCALE);
    }
}

/* ── CRT Scanlines ───────── */
void drawScanlines() {
    for (int y = 0; y < SH; y += 2) {
        drawHLine(0, y, SW, BLACK);
    }
}

#include "tusb.h"

int main() {
    tusb_init();       // start USB stack — do this before your game loop
    initVGA();
    initButtons();
    clearScreen();

    while (true) {
        tud_task();     // drives USB — MUST be called continuously, every loop

        // --- your existing game logic here ---

        // Example: fire button also sends a HID keypress
        static bool wasFiring = false;
        bool firePressed = !gpio_get(BTN_FIRE);

        if (tud_hid_ready()) {
            if (firePressed && !wasFiring) {
                uint8_t keycode[6] = { HID_KEY_SPACE, 0,0,0,0,0 };
                tud_hid_keyboard_report(0, 0, keycode);
            } else if (!firePressed && wasFiring) {
                tud_hid_keyboard_report(0, 0, NULL);   // release
            }
        }
        wasFiring = firePressed;

        // Example: CDC serial debug output
        if (tud_cdc_connected()) {
            tud_cdc_write_str("Frame tick\r\n");
            tud_cdc_write_flush();
        }
    }
}

int main() {

    stdio_init_all();

    initVGA();
    clearScreen();

    int lineHeight = 8 * SCALE;

    int totalHeight = lineHeight * 3 + 20;
    int startY = (SH - totalHeight) / 2;

printCentered("HELLO WORLD 1", startY, RED);
printCentered("HELLO WORLD 2", startY + lineHeight + 10, GREEN);
printCentered("HELLO WORLD 3", startY + (lineHeight + 10) * 2, BLUE);

    drawScanlines();

    while (true) {
        tight_loop_contents();
    }
}