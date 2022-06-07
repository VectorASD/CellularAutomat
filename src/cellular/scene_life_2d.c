#include <scene_life_2d.h>
#include <brain_life_2d.h>

void init_scene_life_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Life2dContext *scene_ctx = malloc(sizeof(struct Life2dContext));

    scene_ctx->map = NULL;
    scene_ctx->panel = get_model_by_id(ctx, 2);
    scene_ctx->ctx = ctx;
    scene_ctx->timer = 0;

    struct Map *cell = get_map_item1(scene_ctx, 0, 0);
    cell->type = (cell->type + 1) % 2;

    scene->user_pointer = scene_ctx;
}

void render_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;

    float *timer = &ctx->timer;
    (*timer) += scene->ctx->time_delta;
    int speed = 1;
    float frequency = 1. / speed;
    while (*timer >= frequency) {
        game_life(ctx);
        (*timer) -= frequency;
    }
}

void gui_scene_life_2d(struct Scene *scene) {
}

void free_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;
    free_map_items1(ctx->map);
}
