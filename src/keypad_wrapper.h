#pragma once
// this file can be used by both C and C++ code
#ifdef __cplusplus
extern "C" {
#endif

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
 * @param brightness A float between 0 and 1 representing the brightness of the keypad
*/
void set_brightness(float brightness);

/**
 * Illuminate a single button
 * 
 * @param x The x coordinate of the button to illuminate
 * @param y The y coordinate of the button to illuminate
 * @param r The red value of the color to illuminate the button with
 * @param g The green value of the color to illuminate the button with
 * @param b The blue value of the color to illuminate the button with
*/
void illuminate(int x, int y, int r, int g, int b);

/**
 * turn off display
*/
void clear();

/**
 * Get the state of the buttons
 * 
 * @return A 16 bit integer representing the state of the buttons
*/
int get_button_states();

/**
 * Update display based on the board state
 * 
 * Given the board state, update every button on the display to reflect the state of the game. 
 * 
 * A board value of 0 corresponds to unlit, 1 to red, and 2 to blue
 * 
 * @param matrix A pointer to an array representing the board state
*/
void set_board(int matrix[4][4]);


#ifdef __cplusplus
}
#endif
