#include <context.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // random, srand
#include <time.h>   // time

void load_context(struct Context *ctx) {
    struct Settings *set = &ctx->settings;
    set->polygon_mod = 0;
    set->user_input_dbg = 0;

    struct Camera *camera = &ctx->camera;
    camera->pos = vector3_new(0, 2, 3);
    camera->front = vector3_new(0, 0, -1);
    camera->up = vector3_new(0, 1, 0);
    camera->speed = 10;
    camera->last_cursor_pos = vector2_new(0, 0);
    camera->yaw = -90;
    camera->pitch = 0;
    camera->sensitivity = 0.2;

    for (short i = 0; i < 1024; i++) ctx->keys[i] = 0;
    for (int key = 0; key < 8; key++) {
        ctx->btn_id_press[key] = -1;
        ctx->btn_id_release[key] = -1;
        ctx->btn_mouse_pressed[key] = 0;
        ctx->btn_mouse_clicked[key] = 0;
    }
    ctx->mouse_lock_mode = 0;
    ctx->mouse_pressed = 0;
    ctx->lock_mouse = 0;

    ctx->scenes = NULL;
    ctx->last_scene = NULL;
    ctx->current_scene = NULL;
    ctx->scenes_n = 0;
    ctx->models = NULL;
    ctx->last_model = NULL;
    ctx->models_n = 0;
    ctx->hovered_part = NULL;

    srand(time(NULL));
    ctx->time_delta = 0;
    ctx->last_frame_time = 0;
    for (int i = 0; i < 6 * 64; i++) ctx->fps_view[0][i] = 0;
    ctx->fps_view_n = 0;

    struct Menus *menus = &ctx->menus;
    menus->show_menu = 0;
    menus->show_menu2 = 0;
}

void upd_projection_mat(struct Context *ctx) {
    mat4 projection = perspective(radians(90), ctx->window_size.z, 0.1, 10000);
    matrix4_push(projection, ctx->projection_loc);
}

void upd_view_mat(struct Context *ctx) {
    struct Camera *camera = &ctx->camera;
    mat4 view = look_at(camera->pos, vector3_add(camera->pos, camera->front), camera->up);
    matrix4_push(view, ctx->view_loc);
}

void do_movement(struct Context *ctx) {
    struct Camera *camera = &ctx->camera;
    float speed = ctx->time_delta * camera->speed;
    if (ctx->keys[GLFW_KEY_LEFT_SHIFT]) speed *= 2;
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

void update_SSBO(struct Context *ctx) {
    struct SSBO {
        GLint founded_id[2];
        GLint buff_id[2];
        GLfloat depth;
        GLint yeah;
    };
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ctx->ssbo);
    struct SSBO *ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    memcpy(ptr->buff_id, ptr->founded_id, sizeof(GLint) * 2);
    size_t addr = *(size_t *) ptr->founded_id;
    ctx->hovered_part = (struct Part *) addr;
    ptr->founded_id[0] = 0;
    ptr->founded_id[1] = 0;
    ptr->depth = 1;
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void context_tick(struct Context *ctx) {
    do_movement(ctx);
    update_SSBO(ctx);
    if (ctx->lock_mouse && ctx->btn_mouse_clicked[0]) {
        ctx->mouse_lock_mode = 1;
        ctx->lock_mouse = 0;
        glUniform1iv(ctx->cursor_pos_loc, 2, ctx->window_center);
        glfwSetInputMode(ctx->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    for (int key = 0; key < 8; key++)
        if (ctx->btn_mouse_clicked[key]) ctx->btn_mouse_clicked[key] = 0;
}

int randint(int a, int b) {
    return rand() % (b - a + 1) + a;
}

float min(float a, float b) {
    return a > b ? b : a;
}
float max(float a, float b) {
    return a > b ? a : b;
}
float line_dist(float x, float y, float x2, float y2) {
    x -= x2;
    y -= y2;
    return sqrt(x * x + y * y);
}
float dot_to_line_dist(float x, float y, float x2, float y2, float dot_x, float dot_y) {
    float a = line_dist(dot_x, dot_y, x, y);
    float b = line_dist(dot_x, dot_y, x2, y2);
    float c = line_dist(x, y, x2, y2);
    if (a * a + c * c >= b * b && b * b + c * c >= a * a) {
        // Ax + By + C = 0
        float A = y - y2;
        float B = x2 - x;
        float C = x * y2 - x2 * y;
        return fabs(A * dot_x + B * dot_y + C) / sqrt(A * A + B * B);
    }
    return min(a, b);
}

short calculate_hovered_button(struct Context *ctx, byte use_dropped_n) {
    float aspect = ctx->window_size.w;
    vec2 *pos = &ctx->camera.last_cursor_pos;
    float x = pos->x * aspect / ctx->window_size.y * 2 - 1;
    float y = 1 - pos->y / ctx->window_size.y * 2;
    struct Primitives *prim = &ctx->prim;
    short id = -1;

    struct VertexList *list = &prim->triangles;
    struct VertexNode *p = list->first, *pred, *pred2;
    int triangles_n = (use_dropped_n ? list->dropped_n : list->n) / 3;
    for (int i = 0; i < triangles_n; i++) {
        pred = p;
        p = p->next;
        pred2 = p;
        p = p->next;
        float a = (pred->x - x) * (pred2->y - pred->y) - (pred2->x - pred->x) * (pred->y - y) > 0 ? 1 : -1;
        float b = (pred2->x - x) * (p->y - pred2->y) - (p->x - pred2->x) * (pred2->y - y) > 0 ? 1 : -1;
        float c = (p->x - x) * (pred->y - p->y) - (pred->x - p->x) * (p->y - y) > 0 ? 1 : -1;
        if (a == b && a == c && p->btn_id > id) id = p->btn_id;
        p = p->next;
    }

    list = &prim->lines;
    p = list->first;
    int lines_n = (use_dropped_n ? list->dropped_n : list->n) / 2;
    for (int i = 0; i < lines_n; i++) {
        pred = p;
        p = p->next;
        if (dot_to_line_dist(pred->x, pred->y, p->x, p->y, x, y) <= 0.01 && p->btn_id > id) id = p->btn_id;
        p = p->next;
    }
    return id;
}

struct VertexNode *get_button_by_id(struct Context *ctx, short btn_id) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *list = &prim->triangles;
    struct VertexNode *p = list->first;
    int triangles_n = list->dropped_n;
    for (int i = 0; i < triangles_n; i++) {
        if (p->btn_id == btn_id) return p;
        p = p->next;
    }
    return NULL;
}

//
// Калбаки всех мастей
//

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
            if (ctx->mouse_lock_mode) {
                ctx->mouse_lock_mode = 0;
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
    struct Camera *camera = &ctx->camera;
    if (set->user_input_dbg) printf("xpos: %3lf   ypos: %3lf\n", xpos, ypos);
    if (ctx->mouse_lock_mode == 0) {
        camera->last_cursor_pos = vector2_new(xpos, ypos);
        GLint pos[2] = {xpos, ctx->window_size.y - 1 - ypos};
        glUniform1iv(ctx->cursor_pos_loc, 2, pos);
        return;
    }
    xpos /= 20;
    ypos /= 20;
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

    glfwSetCursorPos(window, ctx->window_center[0], ctx->window_center[1]);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    struct Context *ctx = glfwGetWindowUserPointer(window);
    struct Settings *set = &ctx->settings;
    if (set->user_input_dbg) printf("button: %d   action: %d   mods: %d\n", button, action, mods);
    short btn_id = calculate_hovered_button(ctx, 1);
    if (action == GLFW_PRESS) {
        ctx->btn_id_press[button] = btn_id;
        ctx->btn_mouse_pressed[button] = 1;
        ctx->mouse_pressed |= 1 << button;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (ctx->mouse_lock_mode == 0 && btn_id == -1) ctx->lock_mouse = 1;
        }
    }
    if (action == GLFW_RELEASE) {
        ctx->btn_id_release[button] = btn_id;
        ctx->btn_mouse_pressed[button] = 0;
        ctx->btn_mouse_clicked[button] = 1;
        if (ctx->mouse_pressed & 1 << button) ctx->mouse_pressed ^= 1 << button;
        if (btn_id >= 0 && btn_id == ctx->btn_id_press[button]) {
            struct VertexNode *btn = get_button_by_id(ctx, btn_id);
            void (*btn_callback)(struct Scene * scene, byte button) = btn->btn_callback;
            ctx->btn_attr = btn->btn_attr;
            btn_callback(ctx->current_scene, button);
        }
        ctx->btn_id_press[button] = -1;
    }
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    struct Context *ctx = glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);
    ctx->window_size = vector4_new(width, height, (float) width / height, (float) height / width);
    ctx->window_center[0] = width / 2;
    ctx->window_center[1] = height / 2;
    upd_projection_mat(ctx);
}
