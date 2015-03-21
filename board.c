
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

#define START_PAWN (i > 7 && i < 16) || (i > 47 && i < 56)
#define START_ROOK i == 0 || i == 7 || i == 56 || i == 63
#define START_KNIGHT i == 1 || i == 6 || i == 57 || i == 62
#define START_BISHOP i == 2 || i == 5 || i == 58 || i == 61
#define START_QUEEN i == 3 || i == 59
#define START_KING i == 4 || i == 60

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

char max(char a, char b) {

    if (a > b) {
        return a;
    }
    return b;
}

char min(char a, char b) {

    if (a < b) {
        return a;
    }
    return b;
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

int check_pawn(char* board, int src, int dest, char turn) {

    int a = 0;
    if (turn) {
        a = src - 8;
    } else {
        a = src + 8;
    }

    if (a == dest) {
        return 1;
    }

    return 0;
}

int check_knight(char *board, int src, int dest) {

    return dest == src + 17
        || dest == src + 15
        || dest == src + 6
        || dest == src + 10
        || dest == src - 17
        || dest == src - 15
        || dest == src - 6
        || dest == src - 10;
}

int check_rook(char *board, int src, int dest) {

    if ((8 + max(dest, src) - min(dest, src)) % 8 == 0) {
        return 1;
    }

    for (int i = src-((src+8)%8)-1; i < (src-((src+8)%8)-1)+(((src+8)%8)+8-1); i++) {
        if (i == dest) {
            return 1;
        }
    }

    return 0;
}

int check_bishop(char *board, int src, int dest) {

    if (dest > src) {
        dest, src = src, dest;
    }

    if ((src - dest) % 7 == 0 || (src - dest) % 9 == 0) {
        return 1;
    }

    return 0;
}

int check_king(char *board, int src, int dest) {
    return src + 1 == dest
        || src - 1 == dest
        || src + 8 == dest
        || src + 7 == dest
        || src + 9 == dest
        || src - 8 == dest
        || src - 7 == dest
        || src - 9 == dest;
}

int can_move(char* board, int src, int dest, char piece, char turn) {


    char p = piece >> 1;

    switch(p) {
    case(PAWN):
        return check_pawn(board, src, dest, turn);
    case(ROOK):
        return check_rook(board, src, dest);
    case(KNIGHT):
        return check_knight(board, src, dest);
    case(BISHOP):
        return check_bishop(board, src, dest);
    case(QUEEN):
        return check_bishop(board, src, dest) || check_rook(board, src, dest);
    case(KING):
        return check_king(board, src, dest);
    default:
        // fail?
        return 0;
    }

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
