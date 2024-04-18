#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keypad_wrapper.h"
#include "pico/stdlib.h"
#include "tic_tac_toe.h"

int main() {
  // game setup
  board* gameboard = malloc(sizeof(board));  // todo should handle errors
  make_board(gameboard);
  player* player1 = malloc(sizeof(player));  // todo
  player* player2 = malloc(sizeof(player));  // todo
  make_player(player1, 1, 1);
  make_player(player2, 2, 0);
  // keypad setup
  init_keypad();
  set_brightness(0.1f);  // this thing is *bright*
  set_board(gameboard->spaces);
  uint16_t last_button_states = 0;

  while (true) {
    // get button input
    button_coords button = get_button_press(&last_button_states);
    if (button.exit_code == 0) {
      if (is_space_valid(gameboard, button.x, button.y)) {
        if (player1->isturn) {
          make_move(player1, gameboard, button.x, button.y);
        } else if (player2->isturn) {
          make_move(player2, gameboard, button.x, button.y);
        }
        set_board(gameboard->spaces);
        next_turn(player1, player2);
      }
    }
  }
  return 0;
}
