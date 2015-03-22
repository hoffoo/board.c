#include "board.h"

char piece_to_char(char p) {

    switch(p >> 1) {
    case PAWN:
        return 'p';
    case ROOK:
        return 'T';
    case KNIGHT:
        return 'k';
    case BISHOP:
        return 'b';
    case QUEEN:
        return 'Q';
    case KING:
        return 'K';
    }
}


void print_board(char* board) {

    for (int i = 0; i < 64; i++) {

        if (i != 0 && i % 8 == 0) {
            printf("\n");
        }

        if (!board[i]) {
            printf("o");
            continue;
        }

        char black = board[i] << 7 | 0;
        if (black) {
            printf("%s%c%s", COLOR_RED, piece_to_char(board[i]), COLOR_RESET);
        } else {
            printf("%s%c%s", COLOR_BLUE, piece_to_char(board[i]), COLOR_RESET);
        }
    }
}

void init_board(char* board) {

    char color = 0;
    for (int i = 0; i < 64; i++) {

        if (i > 47) {
            color = 1;
        }

        if (START_PAWN) {
            board[i] = PAWN << 1 | color;
            continue;
        }

        if (START_ROOK) {
            board[i] = ROOK << 1 | color;
            continue;
        }

        if (START_KNIGHT) {
            board[i] = KNIGHT << 1 | color;
            continue;
        }

        if (START_BISHOP) {
            board[i] = BISHOP << 1 | color;
            continue;
        }

        if (START_QUEEN) {
            board[i] = QUEEN << 1 | color;
            continue;
        }

        if (START_KING) {
            board[i] = KING << 1 | color;
            continue;
        }

        board[i] = 0;
    }
}

// find real positions from coordinates
int coords_to_pos(int x, int y) {

    int row = 0, col = 0, pos = 0;

    for (int i = 0 ; i < 8; i++) {
        for (int k = 0 ; k < 8; k++) {
            if (row == y && col == x) {
                return pos;
            }
            pos++;
            col++;
        }
        row++;
        col = 0;
    }

    return -1;
}
