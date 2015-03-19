
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6

#define COLOR_RED "\x1B[34m"
#define COLOR_BLUE "\x1B[31m"
#define COLOR_RESET "\x1B[0m"

#define DEBUG


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

        if (board[i] == 0) {
            printf("o");
            continue;
        }

        char black = board[i] << 7 | 0;
        if (black) {
            printf(COLOR_RED);
        } else {
            printf(COLOR_BLUE);
        }

        printf("%c", piece_to_char(board[i]));

        printf(COLOR_RESET);
    }
}

void init_board(char* board) {

    char color = 0;
    for (int i = 0; i < 64; i++) {

        if (i > 47) {
            color = 1;
        }

        if ((i > 7 && i < 16) || (i > 47 && i < 56)) {
            board[i] = PAWN << 1 | color;
            continue;
        } 

        if (i == 0 || i == 7 || i == 56 || i == 63) {
            board[i] = ROOK << 1 | color;
            continue;
        }

        if (i == 1 || i == 6 || i == 57 || i == 62) {
            board[i] = KNIGHT << 1 | color;
            continue;
        }

        if (i == 2 || i == 5 || i == 58 || i == 61) {
            board[i] = BISHOP << 1 | color;
            continue;
        }

        if (i == 3 || i == 59) {
            board[i] = QUEEN << 1 | color;
            continue;
        }

        if (i == 4 || i == 60) {
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

int move(char* board, char* dest, int turn) {

    int from_x = -1, from_y = -1, to_x = -1, to_y = -1;
    char str_pos = 0;
    for (int i = 0; i < strlen(dest);i++) {

        if (dest[i] == '\0') {
            break;
        }

        if (dest[i] >= '0' && dest[i] <= '9') {
            switch(str_pos) {
            case 0:
                from_x = dest[i] - '0';
                break;
            case 1:
                from_y = dest[i] - '0';
                break;
            case 2:
                to_x = dest[i] - '0';
                break;
            case 3:
                to_y = dest[i] - '0';
                break;
            }
            str_pos++;
        }
    }

    // check if missing locations
    if (from_x == -1 || from_y == -1 || to_x == -1 || to_y == -1) {
        return 0;
    } 

    // check if same src as dst
    if (from_x == to_x && from_y == to_y) {
        return 0;
    }

    // check if there is a piece
    int pos = coords_to_pos(from_x, from_y);
    if (!board[pos]) {
        printf("no piece here: %d %d\n", from_x, from_y);
        return 0;
    }

    // check if this turn is allowed to move this color piece
    char piece = board[pos];
    if ((piece << 7 & turn << 7) != turn << 7) {
        printf("not your turn to move: %c\n", piece_to_char(piece));
        return 0;
    }
    
    // check if destination is not a of the same color
    int topos = coords_to_pos(to_x, to_y);
    if (board[topos] != 0 && ((board[topos] << 7 & turn << 7) == turn << 7)) {
        printf("cant move there: %c\n", piece_to_char(piece));
        return 0;
    }

    // XXX skipping a ton of checks here
    board[pos] = 0;
    board[topos] = piece;

    // really move the piece

    printf("moving: %c %c%c to %c%c\n", piece_to_char(piece), from_x + '0', from_y + '0', to_x + '0', to_y + '0');
}

int main() {

    char board[8][8] = {};

    init_board(*board);
    print_board(*board);

    printf("\n\n-----------------------------\n\n");

#ifdef DEBUG
    char *inputs[] = {
        "1111", // test same src and dst
        "1012", // test valid move
        "1113", // test move when not my turn
        "0604", // test my turns move
        "1211", // test moving on my own piece
        ""
    };
    int i  = 0;
#endif

    char turn = 0;

    while(1) {
        if (turn) {
            printf("make a move (black): ");
        } else {
            printf("make a move (white): ");
        }
#ifndef DEBUG
        char *line = readline("");
#else
        char *line = inputs[i];
        printf("%s\n", line);
        if (strlen(line) == 0) {
            break;
        }
        i++;
#endif
        if (!move(*board, line, turn)) {
            //printf("invalid move %s\n", line);
        } else {
            turn = turn ^ 1;
            printf("\n\n");
            print_board(*board);
            printf("\n\n");
        }
#ifndef DEBUG
        free(line);
#endif
    }


    return 0;
}
