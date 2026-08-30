/**
 *   CODIGO ORIGINAL BY Hunter Adams (vha3@cornell.edu)
 *   Modificado by San Tarcisio (https://instagram.com)
 *   Modificado para 16 Cores (4-bits 1:2:1 R-G-B Layout)
 * 
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 330 ohm resistor ---> VGA Red
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green (MSB - Bright Green)
 *  - GPIO 20 ---> 680 ohm resistor ---> VGA Green (LSB - Dim Green)
 *  - GPIO 21 ---> 330 ohm resistor ---> VGA Blue
 *  - RP2040 GND ---> VGA GND
 *
 * NOTE
 *  - Connect the outputs of GPIO 19 and GPIO 20 together 
 *    AFTER their respective resistors to feed into the VGA Green Pin.
 */

#pragma once
#include <stdint.h>

// I/O Pin names updated for sequential 4-bit output layout
enum vga_pins {
    HSYNC = 16, 
    VSYNC, 
    RED_PIN,        // GPIO 18 (Bit 0)
    GREEN_MSB_PIN,  // GPIO 19 (Bit 1)
    GREEN_LSB_PIN,  // GPIO 20 (Bit 2)
    BLUE_PIN        // GPIO 21 (Bit 3)
};

// 16 Custom colors mapped to 4-bit hardware binary configurations (B G0 G1 R)
enum colors {
    BLACK          = 0b0000,
    RED            = 0b0001,

    GREEN_DIM      = 0b0100, // Green LSB only
    GREEN_MEDIUM   = 0b0010, // Green MSB only
    GREEN_BRIGHT   = 0b0110, // Both Green pins combined

    BLUE           = 0b1000,

    // Combined shades
    CYAN_DIM       = 0b1100, // Blue + Green Dim
    CYAN_BRIGHT    = 0b1110, // Blue + Full Green
    MAGENTA        = 0b1001, // Blue + Red
    YELLOW_DIM     = 0b0101, // Red + Green Dim
    YELLOW_BRIGHT  = 0b0111, // Red + Full Green
    ORANGE         = 0b0011, // Red + Green Medium

    // Light / Dark variations
    DARK_GRAY      = 0b1010,
    LIGHT_GRAY     = 0b1101,
    PINK           = 0b1011,

    WHITE          = 0b1111  // All pins active
};
// Screen width/height
#define screenWidth 640 // in pixels
#define screenHeight 480 // in pixels

extern unsigned char vga_data_array[];
extern volatile uint32_t currentFrame;  // frame counter

// VGA primitives - usable in main
void initVGA(void);
void clearScreen(void);
void nextFrame(void);
void drawPixel(short x, short y, char color);
void drawVLine(short x, short y, short h, char color);
void drawHLine(short x, short y, short w, char color);
void drawLine(short x0, short y0, short x1, short y1, char color);
void drawRect(short x, short y, short w, short h, char color);
void drawRectCenter(short x, short y, short w, short h, char color);
void drawCircle(short x0, short y0, short r, char color);
void drawCircleHelper(short x0, short y0, short r, unsigned char cornername, char color);
void fillCircle(short x0, short y0, short r, char color);
void fillCircleHelper(short x0, short y0, short r, unsigned char cornername, short delta, char color);
void drawRoundRect(short x, short y, short w, short h, short r, char color);
void fillRoundRect(short x, short y, short w, short h, short r, char color);
void fillRect(short x, short y, short w, short h, char color);
void drawChar(short x, short y, unsigned char c, char color, char bg, unsigned char size);
void setTextCursor(short x, short y);
void setTextColor(char c);
void setTextColor2(char c, char bg);
void setTextSize(unsigned char s);
void setTextWrap(char w);
void tft_write(unsigned char c);
void writeString(char* str);
void drawSprite(short x, short y, const uint8_t* sprite, char color, int scale);

extern const uint8_t player_sprite[8];
extern const uint8_t alien_sprite[8];

// conways
int getPixel(short x, short y);
