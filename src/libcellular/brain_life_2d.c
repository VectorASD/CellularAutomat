#include <brain_life_2d.h>
#include <math.h>

struct MapValue *create_map_item(struct Life2dContext *ctx, int x, int y) {
    struct MapValue item = {x, y, 0, NULL, 0, 0};
    struct MapValue *res = malloc(sizeof(struct MapValue));
    memcpy(res, &item, sizeof(struct MapValue));
    ctx->map_size++;
    return res;
}

void l2d_free_map(struct Map *map) {
    if (map == NULL) return;
    for (byte i = 0; i < 16; i++) l2d_free_map(map->next[i]);
    if (map->level == 0)
        l2d_free_map(map->next_layer);
    else if (map->value != NULL)
        free(map->value);
    free(map);
}

struct Map *new_map_node(struct Life2dContext *ctx, byte level) {
    struct Map *p = malloc(sizeof(struct Map));
    for (byte i = 0; i < 16; i++) p->next[i] = NULL;
    p->value = NULL;
    p->level = level;
    ctx->map_nodes++;
    return p;
}

struct MapValue *l2d_get_cell(struct Life2dContext *ctx, int orig_x, int orig_y) {
    uint x = orig_x >= 0 ? orig_x << 1 : (-orig_x << 1) - 1;
    uint y = orig_y >= 0 ? orig_y << 1 : (-orig_y << 1) - 1;
    struct Map *p = ctx->map;
    if (p == NULL) p = ctx->map = new_map_node(ctx, 0);
    while (x) {
        struct Map **i = p->next + (x & 15);
        if (*i == NULL) *i = new_map_node(ctx, 0);
        p = *i;
        x >>= 4;
    }
    struct Map **i = &p->next_layer;
    if (*i == NULL) *i = new_map_node(ctx, 1);
    p = *i;
    while (y) {
        struct Map **i = p->next + (y & 15);
        if (*i == NULL) *i = new_map_node(ctx, 1);
        p = *i;
        y >>= 4;
    }
    if (p->value == NULL) p->value = create_map_item(ctx, orig_x, orig_y);
    return p->value;
}

void fill_cell(struct Life2dContext *ctx, struct MapValue *map) {
    if (map->type == 1) return;
    map->type = 1;
    int x = map->x, y = map->y;
    if (ctx->create_part) {
        struct Part *part = map->part = ctx->create_part(ctx->ctx, ctx->panel);
        part->pos = vector3_new(x, 0, -y);
        part->orientation.x = radians(90);
        part->color = vector4_new(sin(x / 7.) / 8 + 0.5, sin(y / 11.) / 8 + 0.5, 1, 1);
        ctx->update_part(part);
    }
    ctx->part_count++;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            if (dx || dy) {
                map = l2d_get_cell(ctx, x + dx, y + dy);
                map->neighbours = map->neighbours ? map->neighbours << 1 : 1;
            }
}

void remove_cell(struct Life2dContext *ctx, struct MapValue *map) {
    if (map->type == 0) return;
    map->type = 0;
    if (ctx->delete_part) ctx->delete_part(map->part);
    map->part = NULL;
    ctx->part_count--;
    int x = map->x, y = map->y;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            if (dx || dy) {
                map = l2d_get_cell(ctx, x + dx, y + dy);
                map->neighbours >>= 1;
            }
}

void save_neighbours(struct Map *map) {
    if (map == NULL) return;
    for (byte i = 0; i < 16; i++) save_neighbours(map->next[i]);
    if (map->level == 0)
        save_neighbours(map->next_layer);
    else if (map->value != NULL) {
        struct MapValue *value = map->value;
        value->neighbours2 = value->neighbours;
    }
}

void original_map(struct Life2dContext *ctx) {
    const int Ncells = 50;
    for (int y = -Ncells; y < Ncells; y++)
        for (int x = -Ncells; x < Ncells; x++)
            if (randint(0, 1)) fill_cell(ctx, l2d_get_cell(ctx, x, y));
    save_neighbours(ctx->map);
    ctx->step++;
}

void enumeration_map(struct Life2dContext *ctx, struct Map *map) {
    if (map == NULL) return;
    for (byte i = 0; i < 16; i++) enumeration_map(ctx, map->next[i]);
    if (map->level == 0)
        enumeration_map(ctx, map->next_layer);
    else if (map->value != NULL) {
        struct MapValue *value = map->value;
        byte birth_rule__ = 0b00000100; // рождаются при 3 соседях	|  B3
        byte survive_rule = 0b00000110; // выживают при 2 или 3 соседях | S23
        int neighbours = value->neighbours2;
        if (value->type == 0 && neighbours & birth_rule__) {
            fill_cell(ctx, value);
        } else if (value->type == 1 && !(neighbours & survive_rule)) {
            remove_cell(ctx, value);
        }
    }
}

void map_scrap_collector(struct Life2dContext *ctx, struct Map *pred, byte pos, struct Map *map) {
    if (map == NULL) return;
    for (byte i = 0; i < 16; i++) map_scrap_collector(ctx, map, i, map->next[i]);
    if (map->level == 0)
        map_scrap_collector(ctx, map, 16, map->next_layer);
    else if (map->value != NULL) {
        struct MapValue *value = map->value;
        if (value->part == NULL && value->neighbours == 0) {
            free(map->value);
            map->value = NULL;
            ctx->map_size--;
        }
    }
    if (map->value) return;
    for (byte i = 0; i < 16; i++)
        if (map->next[i]) return;
    if (pos < 16) {
        pred->next[pos] = NULL;
        free(map);
        ctx->map_nodes--;
    } else if (pos == 16) {
        pred->next_layer = NULL;
        free(map);
        ctx->map_nodes--;
    }
}

void game_life(struct Life2dContext *ctx) {
    enumeration_map(ctx, ctx->map);
    save_neighbours(ctx->map);
    map_scrap_collector(ctx, NULL, 17, ctx->map);
    ctx->step++;
}
