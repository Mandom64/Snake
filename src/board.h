#ifndef _BOARD_H
#define _BOARD_H

#define ROWS  25
#define COLS  35
char board[ROWS][COLS];

void draw_borders(char border) {
    for(int i = 0; i < COLS; i++) {
        board[0][i] = border;
        board[ROWS-1][i] = border;
    }
    for(int i = 0; i < ROWS; i++) {
        board[i][0] = border;
        board[i][COLS-1] = border;
    }
}

void init_board() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
        }
    }
    draw_borders('#');
}

void draw_board() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            printw("%c", board[i][j]);
        }
        printw("\n");
    }
}


void clear_board() {
    for(int row = 1; row < ROWS-1; row++) {
        for(int col = 1; col < COLS-1; col++) {
            board[row][col] = ' ';
        }
    }
}

bool is_occupied(int x, int y) {
    if(board[x][y] != ' ')
        return true;
    return false;
}

#endif /* board.h guard */