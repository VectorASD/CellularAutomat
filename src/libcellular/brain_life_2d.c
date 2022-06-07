#include <brain_life_2d.h>
#include <math.h>

struct Map *create_map_item(struct Life2dContext *ctx, int x, int y) {
    struct Part *part = NULL;
    if (ctx->create_part) {
        part = ctx->create_part(ctx->ctx, ctx->panel);
        part->pos = vector3_new(x, 0, -y);
        part->orientation.x = radians(90);
        part->color = vector4_new(sin(x / 7.) / 8 + 0.5, sin(y / 11.) / 8 + 0.5, 1, 1);
        part->visible = 0;
        ctx->update_part(part);
    }
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part, 0, 0};
    struct Map *res = malloc(sizeof(struct Map));
    memcpy(res, &map, sizeof(struct Map));
    ctx->map_size++;
    return res;
}

void l2d_free_map(struct Map *map) {
    if (map == NULL) return;
    l2d_free_map(map->up);
    l2d_free_map(map->left);
    l2d_free_map(map->right);
    l2d_free_map(map->down);
    free(map);
}

struct Map *l2d_get_cell(struct Life2dContext *ctx, int x, int y) {
    if (ctx->map == NULL) return ctx->map = create_map_item(ctx, x, y);
    struct Map *p = ctx->map;
    int level = 1;
    while (1) {
        if (level > ctx->map_height) ctx->map_height = level;
        if (p->x == x && p->y == y) return p;
        if (p->y < y)
            if (p->x < x) {
                if (p->up == NULL) return p->up = create_map_item(ctx, x, y);
                p = p->up;
            } else {
                if (p->left == NULL) return p->left = create_map_item(ctx, x, y);
                p = p->left;
            }
        else if (p->x < x) {
            if (p->right == NULL) return p->right = create_map_item(ctx, x, y);
            p = p->right;
        } else {
            if (p->down == NULL) return p->down = create_map_item(ctx, x, y);
            p = p->down;
        }
        level++;
    }
}

void fill_cell(struct Life2dContext *ctx, struct Map *map) {
    if (map->type == 1) return;
    map->type = 1;
    if (map->part) map->part->visible = 1;
    int x = map->x, y = map->y;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            if (dx || dy) {
                map = l2d_get_cell(ctx, x + dx, y + dy);
                map->neighbours = map->neighbours ? map->neighbours << 1 : 1;
            }
}

void remove_cell(struct Life2dContext *ctx, struct Map *map) {
    if (map->type == 0) return;
    map->type = 0;
    if (map->part) map->part->visible = 0;
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
    save_neighbours(map->up);
    save_neighbours(map->left);
    save_neighbours(map->right);
    save_neighbours(map->down);
    map->neighbours2 = map->neighbours;
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
    enumeration_map(ctx, map->up);
    enumeration_map(ctx, map->left);
    enumeration_map(ctx, map->right);
    enumeration_map(ctx, map->down);
    byte birth_rule__ = 0b00000100; // рождаются при 3 соседях
    byte survive_rule = 0b00000110; // выживают при 2 или 3 соседях
    int neighbours = map->neighbours2;
    if (map->type == 0 && neighbours & birth_rule__) {
        fill_cell(ctx, map);
    } else if (map->type == 1 && !(neighbours & survive_rule)) {
        remove_cell(ctx, map);
    }
}

void game_life(struct Life2dContext *ctx) {
    enumeration_map(ctx, ctx->map);
    save_neighbours(ctx->map);
    ctx->step++;
}
