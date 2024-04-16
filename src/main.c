#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <ncurses/ncurses.h>

#define TICK_RATE 75

typedef struct {
    int x;
    int y;
} Vec2;

/*                  Board stuff                  */
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

int n_rand(int min, int max) {
    return rand() % (max + 1 - min) + min;
}
Vec2 rand_pos() {
    Vec2 r;
    do
    {
        r.x = n_rand(1, COLS-2);
        r.y = n_rand(1, ROWS-2);
    } while (is_occupied(r.x, r.y));
    return r;
}

/*                  Snake stuff                  */
#define MAX_SNAKE_LENGTH ROWS*COLS - 1
typedef struct {
    Vec2 part[MAX_SNAKE_LENGTH];
    Vec2 dir;
    int length;
} Snake;

Snake *snake = NULL;

void init_snake(int spawn_x, int spawn_y) {
    snake = malloc(sizeof(Snake));
    snake->part[0].x = spawn_x;
    snake->part[0].y = spawn_y;
    snake->length = 1;
    snake->dir = (Vec2){.x = 0, .y = -1};
}

void draw_snake() {
    for(int i = 1; i < snake->length; i++) {
        board[snake->part[i].y][snake->part[i].x] = 'o';		
    }
    board[snake->part[0].y][snake->part[0].x] = 'O';
}

void move_snake(int deltaX, int deltaY) {
    snake->dir.x = deltaX;
    snake->dir.y = deltaY;

    // Border check
    if((snake->part[0].x+deltaX) == 0 || 
       (snake->part[0].y+deltaY) == 0 || 
       (snake->part[0].x+deltaX) == COLS-1 || 
       (snake->part[0].y+deltaY) == ROWS-1) {
        return;
    }

    // Shift all the snake parts forward by one
    for(int i = snake->length-1; i > 0; i--) {
        snake->part[i] = snake->part[i-1];
    }
    snake->part[0].x += deltaX;
    snake->part[0].y += deltaY;
}

bool body_collision() {
    for (int i = 1; i < snake->length; i++)
    {
        if(snake->part[0].x == snake->part[i].x &&
           snake->part[0].y == snake->part[i].y) {
            printw("[GAME OVER]: collided with part %d\n", i-1);
            return true;
        }
    }
    return false;
}

/*                  Food stuff                  */
#define MAX_FOODS 4
typedef struct {
    Vec2 list[MAX_FOODS];
    char type[MAX_FOODS];
    bool consumed[MAX_FOODS];
    int ammount;
} Food;

Food *food = NULL;

void spawn_food() {
    food = malloc(sizeof(Food));
    food->ammount = 0;
    for(int i = 0; i < MAX_FOODS; i++) {
        Vec2 r = rand_pos();
        food->list[i].x = r.x;
        food->list[i].y = r.y;
        food->type[i] = n_rand(65, 90);
        food->consumed[i] = false;
        food->ammount++;
    }
}

void draw_food() {
    for(int i = 0; i < MAX_FOODS; i++) {
        if(!food->consumed[i])
            board[food->list[i].y][food->list[i].x] = food->type[i];
    }
}

bool food_collision() {
    for(int i = 0; i < MAX_FOODS; i++) {
        if(snake->part[0].x == food->list[i].x &&
           snake->part[0].y == food->list[i].y) {
            food->consumed[i] = true;
            food->ammount--;
            return true;
           }
    }
    return false;
}

void get_input() {
    switch(getch()) {
        case KEY_UP:
            if(!(snake->dir.x == 0 && snake->dir.y == 1))
                move_snake(0, -1);
            break;
        case KEY_DOWN:
            if(!(snake->dir.x == 0 && snake->dir.y == -1))
                move_snake(0, 1);
            break;
        case KEY_LEFT:
            if(!(snake->dir.x == 1 && snake->dir.y == 0))
                move_snake(-1, 0);
            break;
        case KEY_RIGHT:
            if(!(snake->dir.x == -1 && snake->dir.y == 0))
                move_snake(1, 0);
            break;
    }
    refresh();
}

void game_tick() {
    move_snake(snake->dir.x, snake->dir.y);
    Sleep(TICK_RATE);
}

void cleanup() {
    free(snake);
    free(food);
    endwin();
}


int main(int argc, char **argv) {
    srand(time(NULL));

    // Ncurses stuff
    initscr();
    cbreak();
    noecho(); 
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    // Initialize stuff
    init_board();
    init_snake(COLS/2, ROWS/2);
    spawn_food();
    
    int score = 0;
    // Main game loop
    while (true)
    {
        clear();
        draw_snake(); 
        draw_food();
        draw_borders('#');
        draw_board();
        game_tick();
        get_input();
        if(body_collision()) {
            snake->length = 1;
            score = 0;
        }
        if(food_collision()) {
            score += 100 * (snake->length)/10;
            snake->length++;
        }
        if(food->ammount == 0) {
            spawn_food();
        }
        printw("dir(%d, %d) pos(%d, %d) food left:%d score:%d\n", snake->dir.x, snake->dir.y, 
            snake->part[0].x, snake->part[0].y, food->ammount, score);
        for(int i = 0; i < food->ammount; i++) {
            printw("food %d %c: (%-2d, %-2d)\n", 
                i, food->type[i], food->list[i].x, food->list[i].y);
        }
        clear_board();
        refresh();
    }
    
    cleanup();
    return 0;
}