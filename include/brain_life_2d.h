#pragma once

#include <context.h>

struct MapValue {
    int x, y;
    byte type;
    struct Part *part;
    byte neighbours;
    byte neighbours2;
};

struct Map {
    struct Map *next[16];
    union {
        struct Map *next_layer;
        struct MapValue *value;
    };
    byte level;
};

struct Life2dContext {
    struct Map *map;
    struct Model *panel;
    struct Context *ctx;
    float timer;
    struct Part *(*create_part)(struct Context *ctx, struct Model *model);
    void (*update_part)(struct Part *part);
    void (*delete_part)(struct Part *part);
    int step;
    int map_size, part_count, map_nodes;
};

struct MapValue *create_map_item(struct Life2dContext *ctx, int x, int y);
void l2d_free_map(struct Map *map);
struct MapValue *l2d_get_cell(struct Life2dContext *ctx, int x, int y);
void original_map(struct Life2dContext *ctx);
void game_life(struct Life2dContext *ctx);
