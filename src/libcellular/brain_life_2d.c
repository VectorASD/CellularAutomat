#include <brain_life_2d.h>

const int Ncells = 50;

struct Map *create_cell(struct Life2dContext *ctx, int x, int y) {
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

struct Map *white_cell(struct Life2dContext *ctx, int x, int y) {
    struct Part *part = NULL;
    if (ctx->create_part) {
        part = ctx->create_part(ctx->ctx, ctx->panel);
        part->pos = vector3_new(x, 0, -y);
        part->orientation.x = radians(90);
        part->color = vector4_new(1, 0, 0, 0);
        ctx->update_part(part);
    }
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part};
    struct Map *res = malloc(sizeof(struct Map));
    memcpy(res, &map, sizeof(struct Map));
    return res;
}

void free_map_items1(struct Map *map) {
    if (map == NULL) return;
    free_map_items1(map->up);
    free_map_items1(map->left);
    free_map_items1(map->right);
    free_map_items1(map->down);
    free(map);
}

struct Map *get_map_item1(struct Life2dContext *ctx, int x, int y) {
    if (ctx->map == NULL) return ctx->map = create_cell(ctx, x, y);
    struct Map *p = ctx->map;
    while (1) {
        if (p->x == x && p->y == y) return p;
        if (p->y < y)
            if (p->x < x) {
                if (p->up == NULL) return p->up = create_cell(ctx, x, y);
                p = p->up;
            } else {
                if (p->left == NULL) return p->left = create_cell(ctx, x, y);
                p = p->left;
            }
        else if (p->x < x) {
            if (p->right == NULL) return p->right = create_cell(ctx, x, y);
            p = p->right;
        } else {
            if (p->down == NULL) return p->down = create_cell(ctx, x, y);
            p = p->down;
        }
    }
}

void original_map(struct Life2dContext *ctx) {
    int living_cell;
    for (int x = -Ncells; x < Ncells; x++) {
        for (int y = -Ncells; y < Ncells; y++) {
            living_cell = randint(0, 1);
            if (living_cell == 1) {
                create_cell(ctx, x, y);
            }
        }
    }
}

void game_life(struct Life2dContext *ctx) {
    original_map(ctx);
    int neighbours[Ncells * 2][Ncells * 2];
    for (int x = -Ncells; x < Ncells; x++) {
        for (int y = -Ncells; y < Ncells; y++) {
            for (int c = -1; c < 2; c++) {
                for (int d = -1; d < 2; d++) {
                    if (!(c == 0 && d == 0)) {
                        if (get_map_item1(ctx, x + c, y + d)->type == 0) {
                            neighbours[x][y] += 1;
                        }
                    }
                }
            }
        }
    }
    for (int x = -Ncells; x < Ncells; x++) {
        for (int y = -Ncells; y < Ncells; y++) {
            if (neighbours[x][y] < 2) {
                white_cell(ctx, x, y);
            } else if (neighbours[x][y] == 3) {
                create_cell(ctx, x, y);
            } else if (neighbours[x][y] > 3) {
                white_cell(ctx, x, y);
            }
        }
    }
}
