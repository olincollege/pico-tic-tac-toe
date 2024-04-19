#include "tic_tac_toe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void make_board(board* board_) {
  board_->dim = DIM;
  for (size_t row = 0; row < board_->dim; row++) {
    for (size_t col = 0; col < board_->dim; col++) {
      board_->spaces[row][col] = 0;
    }
  }
}

void make_player(player* player_, int symb, int turn) {
  player_->symb = symb;
  player_->isturn = turn;
}

int is_space_valid(board* board_, size_t row, size_t col) {
  if (row >= board_->dim || col >= board_->dim) {
    return 0;
  }
  if (board_->spaces[row][col] != 0) {
    return 0;
  }
  return 1;
}

int is_full(board* board_) {
  for (size_t row = 0; row < board_->dim; row++) {
    for (size_t col = 0; col < board_->dim; col++) {
      if (board_->spaces[row][col] == 0) {
        return 0;
      }
    }
  }
  return 1;
}

void next_turn(player* player1, player* player2) {
  player1->isturn = !player1->isturn;
  player2->isturn = !player2->isturn;
}

int make_move(player* player_, board* board_, size_t row, size_t col) {
  if (!is_space_valid(board_, row, col) || !player_->isturn) {
    return -1;
  }
  board_->spaces[row][col] = player_->symb;
  return 0;
}

int col_win(player* player_, board* board_) {
  for (size_t col = 0; col < board_->dim; col++) {
    for (size_t row = 0; row < board_->dim; row++) {
      if (board_->spaces[row][col] != player_->symb) {
        break;
      }
      if (row == board_->dim - 1) {
        return 1;
      }
    }
  }
  return 0;
}

int row_win(player* player_, board* board_) {
  for (size_t row = 0; row < board_->dim; row++) {
    for (size_t col = 0; col < board_->dim; col++) {
      if (board_->spaces[row][col] != player_->symb) {
        break;
      }
      if (col == board_->dim - 1) {
        return 1;
      }
    }
  }
  return 0;
}

int diag_win(player* player_, board* board_) {
  for (size_t i = 0; i < board_->dim; i++) {
    if (board_->spaces[i][i] != player_->symb) {
      break;
    }
    if (i == board_->dim - 1) {
      return 1;
    }
  }
  for (size_t i = 0; i < board_->dim; i++) {
    if (board_->spaces[board_->dim -1 - i][i] != player_->symb) {
      break;
    }
    if (i == board_->dim - 1) {
      return 1;
    }
  }

  return 0;
}

int player_win(player* player_, board* board_) {
  return col_win(player_, board_) || row_win(player_, board_) ||
         diag_win(player_, board_);
}
