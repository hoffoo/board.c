#include "board.h"

#define DEBUG

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
    if ((piece | turn) != piece) {
        printf("not your turn to move: %c\n", piece_to_char(piece));
        return 0;
    }

    int topos = coords_to_pos(to_x, to_y);
    // check that position is really on the board
    if (topos == -1) {
        printf("invalid destination: %d %d\n", to_x, to_y);
        return 0;
    }
    // check if destination is not a of the same color
    if (board[topos] != 0 && ((board[topos] | turn) != board[topos])) {
        printf("cant move there: %c\n", piece_to_char(piece));
        return 0;
    }

    // if this piece is allowed to move, move it
    if (can_move(board, pos, topos, piece, turn)) {
        board[pos] = 0;
        board[topos] = piece;
    } else {
        printf("piece cant move there: %c\n", piece_to_char(piece));
        return 0;
    }


    printf("moving: %c %c%c to %c%c\n", piece_to_char(piece), from_x + '0', from_y + '0', to_x + '0', to_y + '0');
}

int main() {

    char board[8][8] = {};

    init_board(*board);
    printf("\n\n-----------------------------\n\n");
    print_board(*board);
    printf("\n\n");


#ifdef DEBUG
    char *inputs[] = {
        "1111 fail white", // test same src and dst
        "0605 ok   black", // test my turns move
        "1211 fail white", // test moving on my own piece
        "1299 fail white", // test moving to an invalid location
        "0102 ok   white", // test moving a pawn
        "0504 ok   black", // test another pawn
        "1021 fail white", // test knight
        "1023 fail white", // test knight
        "1022 ok   white", // test knight
        "1705 ok   black", // test knight
        "0010 ok   white", // test rook
        "1014 ok   white", // test rook
        "1464 ok   white", // test rook
        "6453 fail white", // test rook
        "6454 ok   white", // test rook
        "5455 ok   white", // test rook
        "5515 ok   white", // test rook
        "1516 ok   white", // test rook
        "1607 fail white", // test rook
        "1617 ok   white", // test rook
        "1707 ok   white", // test rook
        "2022 fail white", // test bishop
        "2042 ok   white", // test bishop
        "4252 fail white", // test bishop
        "4250 fail white", // test bishop
        "4252 fail white", // test bishop
        "4253 ok   white", // test bishop
        "5364 ok   white", // test bishop
        "6442 ok   white", // test bishop
        "3034 ok   white", // test queen
        "3424 ok   white", // test queen
        "2433 ok   white", // test queen
        "4030 ok   white", // test king
        "3010 fail white", // test king
        ""
    };
    int i  = 0;
    int failed = -1;
#endif

    char turn = 0;

    while(1) {
#ifdef DEBUG
        char *line = inputs[i];

        if (strstr(inputs[i], "white")) {
            turn = 0;
        } else {
            turn = 1;
        }
#endif
        if (turn) {
            printf("make a move (black): ");
        } else {
            printf("make a move (white): ");
        }
#ifdef DEBUG

        printf("%s\n", line);
        //getc(stdin);
        if (!move(*board, line, turn)) {
            failed = 1;
        } else {
            failed = 0;
#else
        char *line = readline("");
        if (!move(*board, line, turn)) {
        } else {
            turn = turn ^ 1; // swap turn
#endif
            printf("\n\n");
            print_board(*board);
            printf("\n\n");
        }
#ifndef DEBUG
        free(line);
#else
        if (strstr(inputs[i], "fail") != NULL && !failed) {
            printf("\n\nFAIL: %s should have failed\n", inputs[i]);
            return 1;
        }
        if (strstr(inputs[i], "ok") != NULL && failed) {
            printf("\n\nFAIL: %s should have been ok\n", inputs[i]);
            return 1;
        }
        i++;
        if (strlen(inputs[i]) == 0) {
            printf("\n\nSUCCESS: all the moves were as expected!\n");
            return 0;
        }
#endif
    }


    return 0;
}
