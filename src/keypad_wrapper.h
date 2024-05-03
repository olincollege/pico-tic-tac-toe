#pragma once
// this file can be used by both C and C++ code
#ifdef __cplusplus
extern "C" {
    #include <cstdint>
#else
    #include <stdint.h>
#endif

// a struct to hold the x and y coordinates of a button press
typedef struct {
  int exit_code;
  uint8_t x;
  uint8_t y;
} button_coords;

/**
 * Initialize the keypad
 *
 * This function must be called before any other functions in this wrapper
 */
void init_keypad();

/**
 * Update the display
 */
void update_keypad();

/**
 * Set the brightness of the keypad
 *
 * @param brightness A float between 0 and 1 representing the brightness of the
 * keypad
 */
void set_brightness(float brightness);

/**
 * Illuminate a single button
 *
 * @param x The x coordinate of the button
 * @param y The y coordinate of the button
 * @param r Red on a scale of 255.
 * @param g Green on a scale of 255.
 * @param b Blue on a scale of 255.
 */
void illuminate(int x, int y, uint8_t r, uint8_t g, uint8_t b);

/**
 * turn off display
 */
void clear();

/**
 * Get the state of the buttons
 *
 * @return A 16 bit integer representing the state of the buttons
 */
uint16_t get_keypad_state();

/**
 * Update display based on the board state
 *
 * Given the board state, update every button on the display to reflect the
 * state of the game.
 *
 * A board value of 0 corresponds to unlit, 1 to red, and 2 to blue
 *
 * @param matrix An array representing the board state
 */
void set_board(int matrix[4][4]);

/**
 * Set all buttons to a single color
 *
 * @param r Red on a scale of 255.
 * @param g Green on a scale of 255.
 * @param b Blue on a scale of 255.
 */
void set_full_board(uint8_t r, uint8_t g, uint8_t b);

/**
 * Get the location of a button press
 *
 * an exit code of 1 means no button was pressed
 *
 * @param last_button_states A pointer to the last button states bit map
 * @return A button_coords struct containing the x and y coordinates of the
 * button pressed
 */
button_coords get_button_press(uint16_t* last_button_states);

#ifdef __cplusplus
}
#endif
