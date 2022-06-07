#pragma once

#include <context.h>

struct Map {
    struct Map *up, *left, *right, *down;
    int x, y;
    byte type;
    struct Part *part;
    byte neighbours;
};

struct Life2dContext {
    struct Map *map;
    struct Model *panel;
    struct Context *ctx;
    float timer;
    struct Part *(*create_part)(struct Context *ctx, struct Model *model);
    void (*update_part)(struct Part *part);
};

struct Map *create_map_item(struct Life2dContext *ctx, int x, int y);
void l2d_free_map(struct Map *map);
struct Map *l2d_get_cell(struct Life2dContext *ctx, int x, int y);
void original_map(struct Life2dContext *ctx);
void game_life(struct Life2dContext *ctx);
