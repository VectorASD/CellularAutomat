#include <context.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // random, srand
#include <time.h> // time

void load_context(struct Context *ctx) {
    struct Settings *set = &ctx->settings;
    struct Camera *camera = &ctx->camera;
    set->polygon_mod = 0;
    set->user_input_dbg = 0;
    camera->pos = vector3_new(0, 0, 3);
    camera->front = vector3_new(0, 0, -1);
    camera->up = vector3_new(0, 1, 0);
    camera->speed = 5;
    camera->last_cursor_pos = vector2_new(0, 0);
    camera->yaw = -90;
    camera->pitch = 0;
    camera->sensitivity = 0.2;
    for (short i = 0; i < 1024; i++) ctx->keys[i] = 0;
    ctx->delta_time = 0;
    ctx->last_frame_time = 0;
    ctx->lock_mouse_mode = 0;
    ctx->scenes = NULL;
    ctx->last_scene = NULL;
    ctx->current_scene = NULL;
    ctx->scenes_n = 0;
    ctx->models = NULL;
    ctx->last_model = NULL;
    ctx->models_n = 0;
    srand(time(NULL));
}

void upd_projection_mat(struct Context *ctx) {
    mat4 projection = perspective(radians(90), (float) ctx->window_size.x / ctx->window_size.y, 0.1, 100);
    matrix4_push(projection, ctx->projection_loc);
}

void upd_view_mat(struct Context *ctx) {
    struct Camera *camera = &ctx->camera;
    mat4 view = look_at(camera->pos, vector3_add(camera->pos, camera->front), camera->up);
    matrix4_push(view, ctx->view_loc);
}

void do_movement(struct Context *ctx) {
    struct Camera *camera = &ctx->camera;
    float speed = ctx->delta_time * camera->speed;
    if (ctx->keys[GLFW_KEY_LEFT_SHIFT]) speed *= 3;
    vec3 move = vector3_new(0, 0, 0);
    if (ctx->keys[GLFW_KEY_W]) move.x++;
    if (ctx->keys[GLFW_KEY_S]) move.x--;
    if (ctx->keys[GLFW_KEY_A]) move.z--;
    if (ctx->keys[GLFW_KEY_D]) move.z++;
    if (ctx->keys[GLFW_KEY_SPACE]) move.y++;
    if (ctx->keys[GLFW_KEY_LEFT_CONTROL]) move.y--;
    if (move.x || move.y || move.z) {
        move = vector3_norm(move);
        if (move.x) camera->pos = vector3_add(camera->pos, vector3_mul_s(camera->front, speed * move.x));
        if (move.z) camera->pos = vector3_add(camera->pos, vector3_mul_s(vector3_norm(vector3_cross(camera->front, camera->up)), speed * move.z));
        if (move.y) camera->pos = vector3_add(camera->pos, vector3_mul_s(camera->up, speed * move.y));
        upd_view_mat(ctx);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    switch (scancode) {
    case 9: key = GLFW_KEY_ESCAPE; break;
    case 23: key = GLFW_KEY_TAB; break;
    case 37: key = GLFW_KEY_LEFT_CONTROL; break;
    case 50: key = GLFW_KEY_LEFT_SHIFT; break;
    case 65: key = GLFW_KEY_SPACE; break;
    }
    struct Context *ctx = glfwGetWindowUserPointer(window);
    struct Settings *set = &ctx->settings;
    if (set->user_input_dbg) printf("key: %3d   scancode: %2d   action: %d   mode: %2d\n", key, scancode, action, mode);
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            if (ctx->lock_mouse_mode) {
                ctx->lock_mouse_mode = 0;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else
                glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (key == GLFW_KEY_1) {
            set->polygon_mod = (set->polygon_mod + 1) % 3;
            const GLenum polygon_modes[] = {GL_FILL, GL_LINE, GL_POINT};
            glPolygonMode(GL_FRONT_AND_BACK, polygon_modes[set->polygon_mod]);
        }
        if (key == GLFW_KEY_2) printf("Максимальное число входных переменных шейдера этой GPU: %u\n", GL_MAX_VERTEX_ATTRIBS);
        if (key == GLFW_KEY_3) {
            set->user_input_dbg = !set->user_input_dbg;
            printf("Отладка пользовательского ввода успешно: %s\n", set->user_input_dbg ? "Активирована" : "Отключена");
        }
        ctx->keys[key] = 1;
    } else if (action == GLFW_RELEASE)
        ctx->keys[key] = 0;
    else if (action == GLFW_REPEAT)
        ctx->keys[key] = 2;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    struct Context *ctx = glfwGetWindowUserPointer(window);
    struct Settings *set = &ctx->settings;
    if (set->user_input_dbg) printf("xpos: %3lf   ypos: %3lf\n", xpos, ypos);
    if (ctx->lock_mouse_mode == 0) {
        return;
    }
    xpos /= 20;
    ypos /= 20;
    struct Camera *camera = &ctx->camera;
    float dx = xpos - camera->last_cursor_pos.x;
    float dy = camera->last_cursor_pos.y - ypos;
    if (powf(dx * dx + dy * dy, 0.5) > 100) dx = 0, dy = 0;
    camera->yaw += dx * camera->sensitivity;
    camera->pitch += dy * camera->sensitivity;
    camera->last_cursor_pos = vector2_new(xpos, ypos);

    if (camera->pitch > 89) camera->pitch = 89;
    if (camera->pitch < -89) camera->pitch = -89;
    
    float yaw = radians(camera->yaw);
    float pitch = radians(camera->pitch);

    camera->front = vector3_norm(vector3_new(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)));

    upd_view_mat(ctx);

    glfwSetCursorPos(window, ctx->window_size.x / 2, ctx->window_size.y / 2);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    struct Context *ctx = glfwGetWindowUserPointer(window);
    struct Settings *set = &ctx->settings;
    if (set->user_input_dbg) printf("button: %d   action: %d   mods: %d\n", button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (ctx->lock_mouse_mode == 0) {
            ctx->lock_mouse_mode = 1;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    struct Context *ctx = glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);
    ctx->window_size = vector2_new(width, height);
    upd_projection_mat(ctx);
}

int randint(int a, int b) {
    return rand() % (b - a + 1) - a;
}
