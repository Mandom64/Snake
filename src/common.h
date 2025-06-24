#ifndef COMMON_H
#define COMMON_H


typedef struct {
    int x;
    int y;
} Vec2;


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

#endif /* common.h guard */