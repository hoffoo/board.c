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

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void print_board(char* board);
void init_board(char* board);
int coords_to_pos(int x, int y);
char piece_to_char(char p);

int check_pawn(char* board, int src, int dest, char turn);
int check_knight(char *board, int src, int dest);
int check_rook(char *board, int start, int src, int dest);
int check_bishop(char *board, int direction, int src, int dest);
int check_king(char *board, int src, int dest);
int can_move(char* board, int src, int dest, char piece, char turn);
