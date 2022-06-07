#include <brain_ant_2d.h>
#include <scene_ant_2d.h>

void init_scene_ant_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Ant2dContext *scene_ctx = malloc(sizeof(struct Ant2dContext));

    scene_ctx->map = NULL;
    scene_ctx->panel = get_model_by_id(ctx, 2);
    scene_ctx->ctx = ctx;
    scene_ctx->ant_x = scene_ctx->ant_y = 0;
    scene_ctx->ant_rotate = 0;
    scene_ctx->timer = 0;
    scene_ctx->create_part = create_part;
    scene_ctx->update_part = update_part;

    struct Map *cell = get_map_item(scene_ctx, 0, 0);
    cell->type = (cell->type + 1) % 2;

    scene->user_pointer = scene_ctx;
}

void render_scene_ant_2d(struct Scene *scene) {
    struct Ant2dContext *ctx = scene->user_pointer;

    float *timer = &ctx->timer;
    (*timer) += scene->ctx->time_delta;
    int speed = 100;
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
