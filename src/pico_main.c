#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tic_tac_toe.h"


int main(){
    board* gameboard;
    make_board(gameboard);
    player* player1;
    player* player2;
    make_player(player1, 1, 1);
    make_player(player2, 2, 0);

    while(!player_win(player1, gameboard) && !player_win(player2, gameboard)){
        // WAIT FOR PICO INPUT

        // PROCESS INPUT

        // UPDATE BOARD

        // SWITCH TURNS
    }
    return 0;

}
