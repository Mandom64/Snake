#ifndef _FOOD_H
#define _FOOD_H

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

#endif