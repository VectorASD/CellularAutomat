#include <GLFW/glfw3.h>
#include <context.h>

void load_context(struct Context *ctx) {
    struct Settings *set = &ctx->settings;
    struct Camera *camera = &ctx->camera;
    set->polygon_mod = 0;
    set->user_input_dbg = 0;
    camera->pos = vector3_new(0, 0, 3);
    camera->front = vector3_new(0, 0, -1);
    camera->up = vector3_new(0, 1, 0);
    camera->speed = 5;
    for (short i = 0; i < 1024; i++) ctx->keys[i] = 0;
    ctx->delta_time = 0;
    ctx->last_frame_time = 0;
}

void do_movement(struct Context *ctx) {
    struct Camera *camera = &ctx->camera;
    float speed = ctx->delta_time * camera->speed;
    if (ctx->keys[GLFW_KEY_W])
        camera->pos = vector3_add(camera->pos, vector3_mul_s(camera->front, speed));
    if (ctx->keys[GLFW_KEY_S])
        camera->pos = vector3_sub(camera->pos, vector3_mul_s(camera->front, speed));
    if (ctx->keys[GLFW_KEY_A])
        camera->pos = vector3_sub(camera->pos, vector3_mul_s(vector3_norm(vector3_cross(camera->front, camera->up)), speed));
    if (ctx->keys[GLFW_KEY_D])
        camera->pos = vector3_add(camera->pos, vector3_mul_s(vector3_norm(vector3_cross(camera->front, camera->up)), speed));
}
