#include "tic_tac_toe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void make_board(board* board_, size_t dim) {
  board_->dim = dim;
  for (size_t r = 0; r < dim; r++) {
    for (size_t c = 0; c < dim; c++) {
      board_->spaces[r][c] = '_';
    }
  }
}

int is_space_valid(board* board_, size_t r, size_t c) {
  if (r >= board_->dim || r < 0 || c >= board_->dim || c < 0) {
    return 0;
  }
  if (board_->spaces[r][c] != '_') {
    return 0;
  }
  return 1;
}

int is_full(board board_) {
  for (size_t r = 0; r < board_.dim; r++) {
    for (size_t c = 0; c < board_.dim; c++) {
      if (board_.spaces[r][c] == '_') {
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

int make_move(player* player_, board* board_, size_t r, size_t c) {
  if (!is_space_valid(board_, r, c) || !player_->isturn) {
    return -1;
  }
  board_->spaces[r][c] = player_->symb;
}

int col_win(player player_, board board_) {
  for (size_t c = 0; c < board_.dim; c++) {
    for (size_t r = 0; r < board_.dim; r++) {
      if (board_.spaces[r][c] != player_.symb) {
        break;
      }
      if (r == board_.dim - 1) {
        return 1;
      }
    }
  }
  return 0;
}

int row_win(player player_, board board_) {
  for (size_t r = 0; r < board_.dim; r++) {
    for (size_t c = 0; c < board_.dim; c++) {
      if (board_.spaces[r][c] != player_.symb) {
        break;
      }
      if (r == board_.dim - 1) {
        return 1;
      }
    }
  }
  return 0;
}

int diag_win(player player_, board board_) {
  for (size_t i = 0; i < board_.dim; i++) {
    if (board_.spaces[i][i] != player_.symb) {
      break;
    }
    if (i == board_.dim - 1) {
      return 1;
    }
  }
  for (size_t i = 0; i < board_.dim; i++) {
    if (board_.spaces[board_.dim - i][i] != player_.symb) {
      break;
    }
    if (i == board_.dim - 1) {
      return 1;
    }
  }

  return 0;
}

int player_win(player player_, board board_) {
  return col_win(player_, board_) || row_win(player_, board_) ||
         diag_win(player_, board_);
}
