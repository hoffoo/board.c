#include "board.h"

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

int check_rook(char *board, int direction, int src, int dest) {

    if (src > 64 || src < 0) {
        return 0;
    }
    
    if (!direction) {
        return check_rook(board, 1, src, dest)
            || check_rook(board, -1, src, dest)
            || check_rook(board, 8, src, dest)
            || check_rook(board, -8, src, dest);
    }

    int pos = src + direction;
    if (pos < 0) {
        return 0;
    }

    if (board[pos] != 0) {
        return 0;
    } else if (pos == dest) {
        return 1;
    } else if (pos != 0 && (pos%8 == 0 || pos%7 == 0)) {
        return 1;
    }

    return check_rook(board, direction+direction, src, dest);
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

    switch(piece >> 1) {
    case(PAWN):
        return check_pawn(board, src, dest, turn);
    case(ROOK):
        return check_rook(board, 0, src, dest);
    case(KNIGHT):
        return check_knight(board, src, dest);
    case(BISHOP):
        return check_bishop(board, src, dest);
    case(QUEEN):
        return check_bishop(board, src, dest) || check_rook(board, 0, src, dest);
    case(KING):
        return check_king(board, src, dest);
    default:
        // fail?
        return 0;
    }

}
