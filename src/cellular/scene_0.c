#include <math.h> // sin
#include <scene_0.h>
#include <stdlib.h>

struct FirstSceneContext {
    struct Part *part;
    struct Part *part2;
    struct Part *parts[16 * 16 * 16];
    float timer;      // Срабатывает 64 раза в секунду
    float timer2;     // Срабатывает каждые 2 секунды
    int count_timer2; // Сколько раз сработал timer2
};

void init_scene_0(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Model *cube = get_model_by_id(ctx, 0);
    struct Model *sphere = get_model_by_id(ctx, 1);

    struct Part *part = create_part(ctx, cube);
    struct Part *part2 = create_part(ctx, sphere);
    part->size = vector3_new(2, 1, 4);
    part2->pos = vector3_new(0, 3, 0);
    part->color_mode = part2->color_mode = 2;

    struct FirstSceneContext *scene_ctx = malloc(sizeof(struct FirstSceneContext));

    for (int y = 0; y < 16; y++)
        for (int z = 0; z < 16; z++)
            for (int x = 0; x < 16; x++) {
                struct Part *part = create_part(ctx, cube);
                part->pos = vector3_new(x * 1.2, y * 1.2, -z * 1.2 - 3);
                update_part(part);
                part->color = vector4_new(x / 16., y / 16., z / 16., 0.8);
                part->edge_color = vector4_new(1 - x / 16., 1 - y / 16., 1 - z / 16., 1);
                part->visible = (x % 4 == 0) + (y % 4 == 0) + (z % 4 == 0) >= 2;
                scene_ctx->parts[x + y * 16 + z * 256] = part;
            }
    scene_ctx->part = part;
    scene_ctx->part2 = part2;
    scene_ctx->timer = 0;
    scene_ctx->timer2 = 0;
    scene_ctx->count_timer2 = 0;

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
    float size = sin(sin(angle / 4) * 128) + 2;
    part2->size = vector3_new(size, 5 - size, size);
    update_part(part2);
    // отличный пример, как сделать собатия не зависимыми от fps = мощности CPU и GPU в целом!
    scene_ctx->timer += ctx->delta_time;
    scene_ctx->timer2 += ctx->delta_time;
    while (scene_ctx->timer >= 1. / 64) {
        scene_ctx->timer -= 1. / 64;
        struct Part *rand_part = scene_ctx->parts[randint(0, 16 * 16 * 16 - 1)];
        rand_part->visible = !rand_part->visible;
    }
    while (scene_ctx->timer2 >= 2) {
        scene_ctx->timer2 -= 2;
        int t = scene_ctx->count_timer2++;
        for (int y = 0; y < 16; y++)
            for (int z = 0; z < 16; z++)
                for (int x = 0; x < 16; x++)
                    scene_ctx->parts[x + y * 16 + z * 256]->visible = ((x + t) % 4 == 0) + ((y + t) % 4 == 0) + ((z + t) % 4 == 0) >= 2;
    }
}

void btn_callback_0(struct Scene *scene, byte button) {
    text buttons[] = {"ЛКМ", "ПКМ", "СКМ", "доп кнопка #1", "доп кнопка #2", "доп кнопка #3", "доп кнопка #4", "доп кнопка #5"};
    printf("Нажата ПЕРВАЯ кнопка!!! Нажатие гарантируется только тогда, когда курсор был на ней при нажатии И отпускании %s!\n", buttons[button]);
}

void btn_callback_1(struct Scene *scene, byte button) {
    text buttons[] = {"ЛКМ", "ПКМ", "СКМ", "доп кнопка #1", "доп кнопка #2", "доп кнопка #3", "доп кнопка #4", "доп кнопка #5"};
    printf("Нажата ВТОРАЯ кнопка!!! Нажатие гарантируется только тогда, когда курсор был на ней при нажатии И отпускании %s!\n", buttons[button]);
}

void gui_scene_0(struct Scene *scene) {
    struct Context *ctx = scene->ctx;
    struct Primitives *prim = &ctx->prim;
    float color = radians(ctx->time * 16);
    set_text_color(ctx, 191 + sin(color * 4) * 64, 191 + sin(color * 5) * 64, 191 + sin(color * 7) * 64, 255);
    prim->line_color = vector4_new(1, 1, 0, 1);
    prim->line_color2 = vector4_new(0, 0, 1, 0.5);
    prim->tri_color = vector4_new(0, 1, 0, 1);
    prim->tri_color2 = vector4_new(0, 0, 1, 1);
    prim->tri_color3 = vector4_new(1, 1, 0, 1);
    prim->tri_color4 = vector4_new(1, 0.5, 0, 1);
    for (int i = 0; i < 10; i++)
        draw_line(ctx, 100 + i * 15, 100, 500 + i * 15, 500);
    float *alpha2 = &prim->tri_color2.w;
    float *alpha3 = &prim->tri_color3.w;
    *alpha3 = 0;
    draw_triangle(ctx, 100, 300, 100, 500, 300, 500);
    *alpha2 = 0.5;
    draw_box(ctx, 500, 80, 200, 200);
    *alpha2 = *alpha3 = 1;
    draw_box(ctx, 600, 300, 100, 100);
    for (int a = 0; a < 32; a++) {
        float asin = sin(a * M_PI / 16);
        float acos = cos(a * M_PI / 16);
        draw_line(ctx, 650 - acos * 30, 350 - asin * 30, 650 + acos * 150, 350 + asin * 150);
    }
    draw_box(ctx, 680, 220, 100, 100);
    render_text(ctx, "1234567890.,АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\nабвгдеёжзийклмнопрстуфхцчшщъыьэюя!()-+_=:;|~/\\", 5, 5, 24);
    for (int i = 0; i < 6; i++)
        render_text(ctx, ctx->fps_view[(ctx->fps_view_n + i) % 6], 5, 600 - 7 - 15 * (6 - i), 15);

    set_button_color(ctx, 255, 255, 0, 0, 0, 255, 255);
    draw_button(ctx, 100, 100, 100, 64, btn_callback_0);
    set_button_color(ctx, 0, 255, 0, 255, 0, 255, 255);
    draw_button(ctx, 100, 170, 100, 64, btn_callback_1);
}
