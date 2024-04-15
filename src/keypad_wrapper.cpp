#include <cstdint>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico_rgb_keypad.hpp"
#include "keypad_wrapper.h"

// initialize the keypad object
pimoroni::PicoRGBKeypad keypad;

// port default functions from the keypad library
void init_keypad() {
    keypad.init();
}

void update_keypad() {
    keypad.update();
}

void set_brightness(float brightness) {
    keypad.set_brightness(brightness);
}

void illuminate(int x, int y, int r, int g, int b) {
    keypad.illuminate(x, y, r, g, b);
}

void clear() {
    keypad.clear();
}

int get_button_states() {
    return keypad.get_button_states();
}

// getting fancy with new functions 

void set_board(int matrix[4][4]) {
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            int value = matrix[x][y];
            switch(value) {
                case 0:
                keypad.illuminate(x, y, 255, 255, 255);
                    break;
                case 1:
                keypad.illuminate(x, y, 255, 0, 0);
                    break;
                case 2:
                keypad.illuminate(x, y, 0, 0, 255);
                    break;
                default:
                    // green
                    keypad.illuminate(x, y, 0, 255, 0);
            }
        }
    }
    keypad.update();
}


