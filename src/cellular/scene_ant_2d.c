#include <scene_ant_2d.h>

struct Map {
    struct Map *up, *left, *right, *down;
    int x;
    int y;
    byte type;
    struct Part *part;
};

/*
.--------------> X
|  up  | right
| left |  up
|

Y
*/

struct Ant2dContext {
    struct Map *map;
    struct Model *panel;
    struct Context *ctx;
};

struct Map *create_item(struct Ant2dContext *ctx, int x, int y) {
    printf("Create item %d %d\n", x, y);
    struct Part *part = create_part(ctx->ctx, ctx->panel);
    part->pos = vector3_new(x, 0, y);
    part->orientation.x = radians(90);
    update_part(part);
    part->color = vector4_new(1, x / 64. + 0.5, y / 64. + 0.5, 1);
    struct Map map = {NULL, NULL, NULL, NULL, x, y, 0, part};
    struct Map *res = malloc(sizeof(struct Map));
    memcpy(res, &map, sizeof(struct Map));
    return res;
}

void free_map_items(struct Map *map, uint level) {
    if (map == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("item %d %d\n", map->x, map->y);
    free_map_items(map->up, level + 1);
    free_map_items(map->left, level + 1);
    free_map_items(map->right, level + 1);
    free_map_items(map->down, level + 1);
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

    scene->user_pointer = scene_ctx;
}

void render_scene_ant_2d(struct Scene *scene) {
    struct Ant2dContext *ctx = scene->user_pointer;

    get_map_item(ctx, 0, 0);
    for (int i = 0; i < 100; i++) {
        int x = randint(-32, 32), y = randint(-32, 32);
        get_map_item(ctx, x, y);
    }
}

void gui_scene_ant_2d(struct Scene *scene) {
}

void free_scene_ant_2d(struct Scene *scene) {
    printf("free\n");
    struct Ant2dContext *ctx = scene->user_pointer;
    free_map_items(ctx->map, 0);
}
