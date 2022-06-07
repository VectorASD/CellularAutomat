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
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part, 0};
    struct Map *res = malloc(sizeof(struct Map));
    memcpy(res, &map, sizeof(struct Map));
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
    while (1) {
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
    }
}

void fill_cell(struct Life2dContext *ctx, int x, int y) {
    struct Map *map = l2d_get_cell(ctx, x, y);
    if (map->type == 1) return;
    map->type = 1;
    if (map->part) map->part->visible = 1;
    for (int dy = -1; dy < 2; dy++)
        for (int dx = -1; dx < 2; dx++)
            if (dx || dy) {
                byte *neighbours = &l2d_get_cell(ctx, x + dx, y + dy)->neighbours;
                if (*neighbours)
                    (*neighbours) <<= 1;
                else
                    *neighbours = 1;
            }
}

void original_map(struct Life2dContext *ctx) {
    const int Ncells = 50;
    for (int y = -Ncells; y < Ncells; y++)
        for (int x = -Ncells; x < Ncells; x++)
            if (randint(0, 1)) fill_cell(ctx, x, y);
}

void enumeratiun_map(struct Life2dContext *ctx, struct Map *map) {
    if (map == NULL) return;
    enumeratiun_map(ctx, map->up);
    enumeratiun_map(ctx, map->left);
    enumeratiun_map(ctx, map->right);
    enumeratiun_map(ctx, map->down);
    byte birth_rule__ = 0b00000010;   // рождаются при 2 соседях
    byte survive_rule = 0b00000110; // выживают при 2 или 3 соседях
    if (map->type == 0 && map->neighbours & birth_rule__) {
        map->part->color = vector4_new(0, 1, 0, 1);
        map->part->visible = 1;
    } else if (map->type == 1 && !(map->neighbours & survive_rule)) {
        map->part->color = vector4_new(1, 0, 0, 1);
    }
}

void game_life(struct Life2dContext *ctx) {
    enumeratiun_map(ctx, ctx->map);

    /*for (int x = -Ncells; x < Ncells; x++) {
        for (int y = -Ncells; y < Ncells; y++) {
            if (neighbours[x][y] < 2) {
                //white_cell(ctx, x, y);
            } else if (neighbours[x][y] == 3) {
                //create_cell(ctx, x, y);
            } else if (neighbours[x][y] > 3) {
                //white_cell(ctx, x, y);
            }
        }
    }*/
}
