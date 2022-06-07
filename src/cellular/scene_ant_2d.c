#include <scene_ant_2d.h>

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
};

struct Map *create_item(struct Ant2dContext *ctx, int x, int y) {
    struct Part *part = create_part(ctx->ctx, ctx->panel);
    part->pos = vector3_new(x, 0, -y);
    part->orientation.x = radians(90);
    update_part(part);
    part->color = vector4_new(0, 0, 0, 1);
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

void init_scene_ant_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Ant2dContext *scene_ctx = malloc(sizeof(struct Ant2dContext));

    scene_ctx->map = NULL;
    scene_ctx->panel = get_model_by_id(ctx, 2);
    scene_ctx->ctx = ctx;
    scene_ctx->ant_x = scene_ctx->ant_y = 0;
    scene_ctx->ant_rotate = 0;
    scene_ctx->timer = 0;

    struct Map *cell = get_map_item(scene_ctx, 0, 0);
    cell->type = (cell->type + 1) % 2;

    scene->user_pointer = scene_ctx;
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

void render_scene_ant_2d(struct Scene *scene) {
    struct Ant2dContext *ctx = scene->user_pointer;

    float *timer = &ctx->timer;
    (*timer) += scene->ctx->time_delta;
    int speed = 1000;
    float frequency = 1. / speed;
    while (*timer >= frequency) {
        on_walk_ant(ctx);
        (*timer) -= frequency;
    }
}

void gui_scene_ant_2d(struct Scene *scene) {
}

void free_scene_ant_2d(struct Scene *scene) {
    struct Ant2dContext *ctx = scene->user_pointer;
    free_map_items(ctx->map);
}
