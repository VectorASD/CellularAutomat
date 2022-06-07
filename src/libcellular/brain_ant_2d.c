#include <brain_ant_2d.h>

struct Map *create_item(struct Ant2dContext *ctx, int x, int y) {
    struct Part *part = NULL;
    if (ctx->create_part) {
        part = ctx->create_part(ctx->ctx, ctx->panel);
        part->pos = vector3_new(x, 0, -y);
        part->orientation.x = radians(90);
        part->color = vector4_new(0, 0, 0, 1);
        ctx->update_part(part);
    }
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part};
    struct Map *res = malloc(sizeof(struct Map));
    memcpy(res, &map, sizeof(struct Map));
    return res;
}

void free_map_items(struct Map *map) {
    if (map == NULL) return;
    free_map_items(map->up);
    free_map_items(map->left);
    free_map_items(map->right);
    free_map_items(map->down);
    free(map);
}

struct Map *get_map_item(struct Ant2dContext *ctx, int x, int y) {
    if (ctx->map == NULL) return ctx->map = create_item(ctx, x, y);
    struct Map *p = ctx->map;
    while (1) {
        if (p->x == x && p->y == y) return p;
        if (p->y < y)
            if (p->x < x) {
                if (p->up == NULL) return p->up = create_item(ctx, x, y);
                p = p->up;
            } else {
                if (p->left == NULL) return p->left = create_item(ctx, x, y);
                p = p->left;
            }
        else if (p->x < x) {
            if (p->right == NULL) return p->right = create_item(ctx, x, y);
            p = p->right;
        } else {
            if (p->down == NULL) return p->down = create_item(ctx, x, y);
            p = p->down;
        }
    }
}

void on_walk_ant(struct Ant2dContext *ctx) {
    int *x = &ctx->ant_x, *y = &ctx->ant_y;
    byte *rotate = &ctx->ant_rotate;
    //printf("%d %d %u\n", *x, *y, *rotate);
    switch (*rotate) {
    case 0: (*y)++; break;
    case 1: (*x)++; break;
    case 2: (*y)--; break;
    case 3: (*x)--; break;
    }
    struct Map *cell = get_map_item(ctx, *x, *y);
    *rotate = (*rotate + (cell->type ? 3 : 1)) % 4;
    cell->type = (cell->type + 1) % 2;
    switch (cell->type) {
    case 0: cell->part->color = vector4_new(*x / 64. + 0.5, *y / 64. + 0.5, 1, 1); break;
    case 1: cell->part->color = vector4_new(0, 0, 0, 1); break;
    }
}
