#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "../src/tic_tac_toe.h"

// Test making a board
Test(make_board, make_a_board) {
  board board_;
  make_board(&board_);
  cr_assert(eq(int, board_.dim, DIM));
  for (size_t row = 0; row < board_.dim; row++) {
    for (size_t col = 0; col < board_.dim; col++) {
      cr_assert(eq(int, board_.spaces[row][col], 0));
    }
  }
}

// Test making a player
Test(make_player, make_a_player) {
  player player_;
  make_player(&player_, 1, 0);
  cr_assert(eq(int, player_.symb, 1));
  cr_assert(eq(int, player_.isturn, 0));
}

// Test top left square
Test(is_space_valid, top_left_empty) {
  board board_;
  make_board(&board_);
  int check = is_space_valid(&board_, 0, 0);
  cr_assert(eq(int, check, 1));
}

// Test bottom right square
Test(is_space_valid, bottom_right_empty) {
  board board_;
  make_board(&board_);
  int check = is_space_valid(&board_, 3, 3);
  cr_assert(eq(int, check, 1));
}

// Test out of bounds
Test(is_space_valid, pos_out_of_bounds) {
  board board_;
  make_board(&board_);
  const size_t idx = 5;
  int check = is_space_valid(&board_, idx, idx);
  cr_assert(eq(int, check, 0));
}

// Test occupied space
Test(is_space_valid, occupied) {
  board board_;
  make_board(&board_);
  board_.spaces[1][2] = 2;
  int check = is_space_valid(&board_, 1, 2);
  cr_assert(eq(int, check, 0));
}

// Test empty board
Test(is_full, empty) {
  board board_;
  make_board(&board_);
  int check = is_full(&board_);
  cr_assert(eq(int, check, 0));
}

// Test partially filled board
Test(is_full, partial) {
  board board_;
  make_board(&board_);
  board_.spaces[1][0] = 1;
  board_.spaces[3][0] = 2;
  board_.spaces[0][1] = 1;
  board_.spaces[2][1] = 2;
  board_.spaces[1][2] = 1;
  board_.spaces[3][2] = 2;
  board_.spaces[0][3] = 1;
  board_.spaces[2][3] = 2;
  int check = is_full(&board_);
  cr_assert(eq(int, check, 0));
}

// Test filled board
Test(is_full, filled) {
  board board_;
  make_board(&board_);
  for (size_t row = 0; row < board_.dim; row++) {
    for (size_t col = 0; col < board_.dim; col++) {
      board_.spaces[row][col] = 1;
    }
  }
  int check = is_full(&board_);
  cr_assert(eq(int, check, 1));
}

// Test toggling turns
Test(next_turn, toggle) {
  player player1;
  make_player(&player1, 1, 0);
  player player2;
  make_player(&player2, 2, 1);
  next_turn(&player1, &player2);
  cr_assert(eq(int, player1.isturn, 1));
  cr_assert(eq(int, player2.isturn, 0));
  next_turn(&player1, &player2);
  cr_assert(eq(int, player1.isturn, 0));
  cr_assert(eq(int, player2.isturn, 1));
}

// Test claiming an empty space
Test(make_move, empty_space) {
  board board_;
  make_board(&board_);
  player player_;
  make_player(&player_, 2, 1);
  int check = make_move(&player_, &board_, 3, 2);
  cr_assert(eq(int, board_.spaces[3][2], 2));
  cr_assert(eq(int, check, 0));
}

// Test claiming a filled space
Test(make_move, filled) {
  board board_;
  make_board(&board_);
  board_.spaces[3][2] = 1;
  player player_;
  make_player(&player_, 2, 1);
  int check = make_move(&player_, &board_, 3, 2);
  cr_assert(eq(int, board_.spaces[3][2], 1));
  cr_assert(eq(int, check, -1));
}

// Test claiming an out of bounds space
Test(make_move, out_of_bounds) {
  board board_;
  make_board(&board_);
  player player_;
  make_player(&player_, 2, 1);
  int check = make_move(&player_, &board_, 3, 4);
  cr_assert(eq(int, check, -1));
}

// Test finding a row win
Test(player_win, row) {
  board board_;
  make_board(&board_);
  player player1;
  make_player(&player1, 1, 0);
  player player2;
  make_player(&player2, 2, 1);
  for (size_t col = 0; col < board_.dim; col++) {
    board_.spaces[2][col] = 1;
  }
  int check = player_win(&player1, &board_);
  cr_assert(eq(int, check, 1));
}

// Test finding a column win
Test(player_win, col) {
  board board_;
  make_board(&board_);
  player player1;
  make_player(&player1, 1, 0);
  player player2;
  make_player(&player2, 2, 1);
  for (size_t row = 0; row < board_.dim; row++) {
    board_.spaces[row][1] = 2;
  }
  int check = player_win(&player2, &board_);
  cr_assert(eq(int, check, 1));
}

// Test finding a diagonal win
Test(player_win, diag) {
  board board_;
  make_board(&board_);
  player player1;
  make_player(&player1, 1, 0);
  player player2;
  make_player(&player2, 2, 1);
  for (size_t col = 0; col < board_.dim; col++) {
    board_.spaces[col][board_.dim - 1 - col] = 1;
  }
  int check = player_win(&player1, &board_);
  cr_assert(eq(int, check, 1));
}

// Test other player win
Test(player_win, loss) {
  board board_;
  make_board(&board_);
  player player1;
  make_player(&player1, 1, 0);
  player player2;
  make_player(&player2, 2, 1);
  for (size_t col = 0; col < board_.dim; col++) {
    board_.spaces[col][col] = 1;
  }
  int check = player_win(&player2, &board_);
  cr_assert(eq(int, check, 0));
}
