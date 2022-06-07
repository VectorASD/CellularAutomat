#include <brain_life_2d.h>
#include <scene_life_2d.h>

void init_scene_life_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Life2dContext *scene_ctx = malloc(sizeof(struct Life2dContext));

    scene_ctx->map = NULL;
    scene_ctx->panel = get_model_by_id(ctx, 2);
    scene_ctx->ctx = ctx;
    scene_ctx->timer = 0;
    scene_ctx->create_part = create_part;
    scene_ctx->update_part = update_part;
    scene_ctx->step = 0;
    scene_ctx->map_size = scene_ctx->map_height = 0;

    original_map(scene_ctx);

    scene->user_pointer = scene_ctx;
}

void render_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;

    ctx->timer += scene->ctx->time_delta;
    ctx->timer2 += scene->ctx->time_delta;
    int speed = 100;
    float frequency = 1. / speed;
    while (ctx->timer >= frequency) {
        game_life(ctx);
        ctx->timer -= frequency;
    }
    while (ctx->timer2 >= 1) {
        printf("map_size: %8u    map_height: %8u     %s\n", ctx->map_size, ctx->map_height, ctx->map_height >= 1000 ? "!!!" : "");
        ctx->timer2 -= 1;
    }
}

void gui_scene_life_2d(struct Scene *scene) {
}

void free_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;
    l2d_free_map(ctx->map);
}
