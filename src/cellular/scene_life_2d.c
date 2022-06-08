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
    scene_ctx->delete_part = delete_part;
    scene_ctx->step = 0;
    scene_ctx->map_size = scene_ctx->part_count = scene_ctx->map_nodes = 0;

    original_map(scene_ctx);

    scene->user_pointer = scene_ctx;
}

void render_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;
    ctx->timer += scene->ctx->time_delta;
    int speed = 100;
    float frequency = 1. / speed;
    while (ctx->timer >= frequency) {
        game_life(ctx);
        ctx->timer -= frequency;
    }
}

void gui_scene_life_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Life2dContext *scene_ctx = scene->user_pointer;

    set_text_alignment(ctx, 0, 2, 0);
    set_text_color(ctx, 0, 0, 255, 255);
    render_text(ctx, "Нодусов карты:", 5, 600 - 50, 15);
    render_text(ctx, "Размер карты:", 5, 600 - 35, 15);
    render_text(ctx, "Всего клеток:", 5, 600 - 20, 15);

    set_text_alignment(ctx, 2, 2, 0);
    set_text_color(ctx, 255, 255, 0, 255);
    char buff[128];
    sprintf(buff, "%6u", scene_ctx->map_nodes);
    render_text(ctx, buff, 170, 600 - 50, 15);
    sprintf(buff, "%6u", scene_ctx->map_size);
    render_text(ctx, buff, 170, 600 - 35, 15);
    sprintf(buff, "%6u", scene_ctx->part_count);
    render_text(ctx, buff, 170, 600 - 20, 15);
}

void free_scene_life_2d(struct Scene *scene) {
    struct Life2dContext *ctx = scene->user_pointer;
    l2d_free_map(ctx->map);
}
