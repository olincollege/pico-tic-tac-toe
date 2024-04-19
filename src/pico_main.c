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
  player* player1 = malloc(sizeof(player));
  player* player2 = malloc(sizeof(player));
  make_board(gameboard);
  make_player(player1, 1, 1);
  make_player(player2, 2, 0);
  int game_over = 0;  // 0 if not over, 1 if p1 wins, 2 in p2 wins, 3 if tie
  // keypad setup
  init_keypad();
  set_brightness(0.1f);  // this thing is *bright*
  set_board(gameboard->spaces);
  uint16_t last_button_states = 0;

  while (true) {
    if (game_over == 0) {
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
          // check for win
          if (player_win(player1, gameboard)) {
            game_over = 1;
          } else if (player_win(player2, gameboard)) {
            game_over = 2;
          } else if (is_full(gameboard)) {
            game_over = 3;
          } else {
            next_turn(player1, player2);
          }
        }
      }
    } else {
      // game over
      // turn full board to winning color
      if (game_over == 1) {
        set_full_board(255, 0, 0);
      } else if (game_over == 2) {
        set_full_board(0, 0, 255);
      } else if (game_over == 3) {
        sleep_ms(500);  // see the last move
        set_full_board(0, 255, 0);
      }
      // pause for a sec
      sleep_ms(1000);
      // set board back to normal

      // reset game
      make_board(gameboard);
      set_board(gameboard->spaces);
      make_player(player1, 1, 1);
      make_player(player2, 2, 0);
      game_over = 0;
    }
  }
  return 0;
}
