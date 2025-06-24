#ifndef _SNAKE_H
#define _SNAKE_H

#define MAX_SNAKE_LENGTH ROWS * COLS - 1

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

#endif /* snake.h guard */