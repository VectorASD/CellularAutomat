#include <brain_life_2d.h>

struct Map *create_map_item(struct Life2dContext *ctx, int x, int y) {
    struct Part *part = NULL;
    if (ctx->create_part) {
        part = ctx->create_part(ctx->ctx, ctx->panel);
        part->pos = vector3_new(x, 0, -y);
        part->orientation.x = radians(90);
        part->color = vector4_new(x / 64., y / 64., 1, 1);
        part->visible = 0;
        ctx->update_part(part);
    }
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part};
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
}

void original_map(struct Life2dContext *ctx) {
    const int Ncells = 50;
    for (int y = -Ncells; y < Ncells; y++)
        for (int x = -Ncells; x < Ncells; x++)
            if (randint(0, 1)) fill_cell(ctx, x, y);
}

void game_life(struct Life2dContext *ctx) {
    const int Ncells = 50;
    int neighbours[Ncells * 2][Ncells * 2];
    for (int x = -Ncells; x < Ncells; x++)
        for (int y = -Ncells; y < Ncells; y++)
            for (int dy = -1; dy < 2; dy++)
                for (int dx = -1; dx < 2; dx++)
                    if (dx || dy)
                        if (l2d_get_cell(ctx, x + dx, y + dy)->type == 0)
                            neighbours[x][y] += 1;

    for (int x = -Ncells; x < Ncells; x++) {
        for (int y = -Ncells; y < Ncells; y++) {
            if (neighbours[x][y] < 2) {
                //white_cell(ctx, x, y);
            } else if (neighbours[x][y] == 3) {
                //create_cell(ctx, x, y);
            } else if (neighbours[x][y] > 3) {
                //white_cell(ctx, x, y);
            }
        }
    }
}
