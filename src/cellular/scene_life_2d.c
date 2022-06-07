#include <brain_life_2d.h>
#include <scene_life_2d.h>

void init_scene_life_2d(struct Scene *scene) {
    printf("A\n");
    struct Context *ctx = scene->ctx;
    struct Life2dContext *scene_ctx = malloc(sizeof(struct Life2dContext));

    scene_ctx->map = NULL;
    scene_ctx->panel = get_model_by_id(ctx, 2);
    scene_ctx->ctx = ctx;
    scene_ctx->timer = 0;
    scene_ctx->create_part = create_part;
    scene_ctx->update_part = update_part;

    original_map(scene_ctx);

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
    l2d_free_map(ctx->map);
}
