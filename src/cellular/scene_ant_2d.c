#include <scene_ant_2d.h>

struct Ant2dContext {
    struct Part *panels[100 * 100];
};

void init_scene_ant_2d(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Ant2dContext *scene_ctx = malloc(sizeof(struct Ant2dContext));

    struct Model *panel = get_model_by_id(ctx, 2);
    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++) {
            struct Part *part = create_part(ctx, panel);
            part->pos = vector3_new(x - 50, 0, y - 50);
            part->orientation.x = radians(90);
            update_part(part);
            part->color = vector4_new(x / 99., y / 99., 1, 0.8);
            scene_ctx->panels[x + y * 100] = part;
        }

    scene->user_pointer = scene_ctx;
}

void render_scene_ant_2d(struct Scene *scene) {
}

void gui_scene_ant_2d(struct Scene *scene) {
}
