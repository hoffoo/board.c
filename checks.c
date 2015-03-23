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
    
    if (!direction) {
        return check_rook(board, 1, src, dest)
            || check_rook(board, -1, src, dest)
            || check_rook(board, 8, src, dest)
            || check_rook(board, -8, src, dest);
    }


    int pos = src + direction;
    if (pos < 0 || pos > 64) {
        return 0;
    }

    // going off the edge of the chessboard
    if (pos-1 != 0 && (pos-1%8 == 0 || pos-1%7 == 0)) {
        return 0;
    }

    // found the right destination
    if (pos == dest) {
        return 1;
    }

    // not an empty position
    if (board[pos] != 0) {
        return 0;
    }

    src = pos;

    return check_rook(board, direction, src, dest);
}

int check_bishop(char *board, int direction, int src, int dest) {

    if (!direction) {
        return check_bishop(board,  7, src, dest)
            || check_bishop(board, -7, src, dest)
            || check_bishop(board,  9, src, dest)
            || check_bishop(board, -9, src, dest);
    }

    int pos = src + direction;
    if (pos < 0 || pos > 64) {
        return 0;
    }

    // going off the edge of the chessboard
    if (pos-1 != 0 && (pos-1%8 == 0 || pos-1%7 == 0)) {
        return 0;
    }

    // found the right destination
    if (pos == dest) {
        return 1;
    }

    // not an empty position
    if (board[pos] != 0) {
        return 0;
    }

    src = pos;

    return check_bishop(board, direction, src, dest);
}

int check_king(char *board, int src, int dest) {

    return src + 1 == dest
        || src + 8 == dest
        || src + 7 == dest
        || src + 9 == dest
        || src - 1 == dest
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
        return check_bishop(board, 0, src, dest);
    case(QUEEN):
        return check_bishop(board, 0, src, dest) || check_rook(board, 0, src, dest);
    case(KING):
        return check_king(board, src, dest);
    default:
        // fail?
        return 0;
    }

}
