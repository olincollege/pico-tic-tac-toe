#include <stdlib.h>

enum { DIM = 4 };

typedef struct {
  int symb;
  int isturn;
} player;

typedef struct {
  int spaces[DIM][DIM];
  size_t dim;
} board;

/**
 * Create a blank board
 *
 * Initilize the board spaces array of arrays as all blank and store the
 * dimension as an easily accessible quantity.
 * @param board_ A pointer to the board to set up
 */
void make_board(board* board_);

/**
 * Create a player
 *
 * Given an integer symbol and turn, initialize a player with that symbol and
 * turn. Turn should only be 0 or 1.
 * @param player_ A pointer to the player to set
 * @param symb The symbol the player will be represented with
 * @param turn 0 if it is not the player's turn, 1 if it is
 */
void make_player(player* player_, int symb, int turn);

/**
 * Check if a space can be taken by a player
 *
 * Given a board and row and column coordinates, verify that that space is both
 * within the dimensions of the board and is not already occupied. Return 1 if a
 * player can claim the space, return 0 if they cannot.
 * @param board_ A pointer to the board to check
 * @param r The row of the space to check
 * @param c The column of the space to check
 * @return 1 if the space is valid, 0 if it is not
 */
int is_space_valid(board* board_, size_t row, size_t col);

/**
 * Check if the board has been filled entirely
 *
 * Given a board, check if there are no remaining blank spaces. Return 0 if the
 * board has empty space(s), 1 if it is full.
 * @param board_ A pointer to the board to check
 * @return 1 if the board is full, 0 if it is not
 */
int is_full(board* board_);

/**
 * Go to the next turn in the game
 *
 * Given two players, invert the turn of them both. If player1's turn is 1 and
 * player2's turn is 0, on the next turn, their turns will be 0 and 1,
 * respectively.
 * @param player1 A pointer to the first player
 * @param player2 A pointer to the second player
 */
void next_turn(player* player1, player* player2);

/**
 * Have a player make a move
 *
 * Given a player, a board, and a space location, first check if the move is
 * possible. If it is not, return -1. If it is, update the board with the
 * player's symbol in the requested space.
 * @param player_ A pointer to the player making a move
 * @param board_ A pointer to the game board in use
 * @param r The row of the space to take
 * @param c The column of the space to take
 * @return 0 if the move is successful, -1 if it is impossible
 */
int make_move(player* player_, board* board_, size_t row, size_t col);

/**
 * Check if a player has filled a column
 *
 * Given a player and a board, go down each column until a column filled with
 * the player's sybmol is found, or until the end of the board is reached. If a
 * winning column is found, return 1. If the last column is read with no win
 * found, return 0.
 * @param player_ A pointer to the board to check
 * @param board_ A pointer to the board to check
 * @return 1 if a win is found, 0 if it is not
 */
int col_win(player* player_, board* board_);

/**
 * Check if a player has filled a row
 *
 * Given a player and a board, go across each row until a row filled with the
 * player's sybmol is found, or until the end of the board is reached. If a
 * winning row is found, return 1. If the last row is read with no win found,
 * return 0.
 * @param player_ A pointer to the board to check
 * @param board_ A pointer to the board to check
 * @return 1 if a win is found, 0 if it is not
 */
int row_win(player* player_, board* board_);

/**
 * Check if a player has filled a diagonal
 *
 * Given a player and a board, go across both diagonaly until a diagonal filled
 * with the player's sybmol is found, or until the end of the board is reached.
 * If a winning diagonal is found, return 1. If both diagonals are read with no
 * win found, return 0.
 * @param player_ A pointer to the board to check
 * @param board_ A pointer to the board to check
 * @return 1 if a win is found, 0 if it is not
 */
int diag_win(player* player_, board* board_);

/**
 * Check if a player has won
 *
 * Given a board, go across each row until a row filled with the player's sybmol
 * is found, or until the end of the board is reached. If a winning row is
 * found, return 1. If the last row is read with no win found, return 0.
 * @param player_ A pointer to the board to check
 * @param board_ A pointer to the board to check
 * @return 1 if a win is found, 0 if it is not
 */
int player_win(player* player_, board* board_);
