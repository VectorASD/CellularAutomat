#pragma once

#include <context.h>

struct Map {
    struct Map *up, *left, *right, *down;
    int x;
    int y;
    byte type;
    struct Part *part;
};

struct Ant2dContext {
    struct Map *map;
    struct Model *panel;
    struct Context *ctx;
    int ant_x;
    int ant_y;
    byte ant_rotate;
    float timer;
    struct Part *(*create_part)(struct Context *ctx, struct Model *model); // равен NULL при тестировании, т.к. система сцен не может входить в библиотеку
    void (*update_part)(struct Part *part);
};

struct Map *create_item(struct Ant2dContext *ctx, int x, int y);
void free_map_items(struct Map *map);
struct Map *get_map_item(struct Ant2dContext *ctx, int x, int y);
void on_walk_ant(struct Ant2dContext *ctx);
