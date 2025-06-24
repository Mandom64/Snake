
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include "board.h"
#include "common.h"
#include "snake.h"
#include "food.h"

#ifdef _WIN32
    #include <windows.h>
    #define sleep(time) Sleep(time)
    #define MS 0
#else
    #include <unistd.h>
    #define sleep(time) usleep(time)
    #define MS 1000
#endif

#define TICKS 75
#define TICK_RATE (TICKS * MS)

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
    sleep(TICK_RATE);
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