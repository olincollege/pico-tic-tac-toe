#include "keypad_wrapper.h"

#include <stdio.h>

#include <cstdint>

#include "pico/stdlib.h"
#include "pico_rgb_keypad.hpp"

// initialize the keypad object
pimoroni::PicoRGBKeypad keypad;

// port default functions from the keypad library
void init_keypad() { keypad.init(); }

void update_keypad() { keypad.update(); }

void set_brightness(float brightness) { keypad.set_brightness(brightness); }

void illuminate(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  keypad.illuminate(x, y, r, g, b);
}

void clear() { keypad.clear(); }

uint16_t get_keypad_state() { return keypad.get_button_states(); }

// getting fancy with new functions

void set_board(int matrix[4][4]) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      int value = matrix[x][y];
      switch (value) {
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

void set_full_board(uint8_t r, uint8_t g, uint8_t b) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      keypad.illuminate(x, y, r, g, b);
    }
  }
  keypad.update();
}

button_coords get_button_press(uint16_t* last_button_states) {
  button_coords coords;
  coords.exit_code = 1;  // exit code 1 means no button was pressed
  uint16_t button_states = get_button_states();
  if (button_states != *last_button_states) {
    for (int i = 0; i < 16; i++) {
      if (button_states & (1 << i)) {  // get index of button pressed
        coords.y = i / 4;              // works because of integer division
        coords.x = i % 4;
        coords.exit_code = 0;
      }
    }
  }
  *last_button_states = button_states;
  return coords;
}
