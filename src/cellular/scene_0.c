#include <scene_0.h>
#include <stdlib.h>

struct FirstSceneContext {
    struct Part *part;
    struct Part *part2;
};

void init_scene_0(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Model *cube = get_model_by_id(ctx, 0);
    struct Model *sphere = get_model_by_id(ctx, 1);

    struct Part *part = create_part(ctx, cube);
    struct Part *part2 = create_part(ctx, sphere);
    part->size = vector3_new(2, 1, 4);
    part2->size = vector3_new(1, 5, 1);
    part2->pos = vector3_new(0, 3, 0);
    part->color_mode = part2->color_mode = 2;

    for (int y = 0; y < 16; y++)
        for (int z = 0; z < 16; z++)
            for (int x = 0; x < 16; x++) {
                struct Part *part = create_part(ctx, cube);
                part->pos = vector3_new(x * 2, y * 2, -z * 2 - 2);
                update_part(part);
                part->color_mode = (x + y + z) % 4;
                part->color = vector4_new(x / 16., y / 16., z / 16., 1);
                part->edge_color = vector4_new(1 - x / 16., 1 - y / 16., 1 - z / 16., 1);
                part->visible = (x % 5 == 1) + (y % 5 == 2) + (z % 5 == 3) >= 2;
            }

    struct FirstSceneContext *scene_ctx = malloc(sizeof(struct FirstSceneContext));
    scene_ctx->part = part;
    scene_ctx->part2 = part2;
    scene->user_pointer = scene_ctx;
}

void render_scene_0(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct FirstSceneContext *scene_ctx = scene->user_pointer;
    struct Part *part = scene_ctx->part;
    struct Part *part2 = scene_ctx->part2;

    float angle = radians(ctx->time * 50);
    part->orientation.y = angle;
    update_part(part);
    part2->orientation.x = angle;
    part2->orientation.y = angle / 5;
    update_part(part2);
}
