#pragma once

#include <context.h>

struct Map {
    struct Map *up, *left, *right, *down;
    int x;
    int y;
    byte type;
    struct Part *part;
};

struct Life2dContext {
    struct Map *map;
    struct Model *panel;
    struct Context *ctx;
    float timer;
    struct Part *(*create_part)(struct Context *ctx, struct Model *model);
    void (*update_part)(struct Part *part);
};

struct Map *create_cell(struct Life2dContext *ctx, int x, int y);
struct Map *white_cell(struct Life2dContext *ctx, int x, int y);
void free_map_items1(struct Map *map);
struct Map *get_map_item1(struct Life2dContext *ctx, int x, int y);
void original_map(struct Life2dContext *ctx);
void game_life(struct Life2dContext *ctx);
