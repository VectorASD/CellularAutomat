#pragma once

#include <matrix.h>

typedef unsigned char byte;
typedef const char *text;

struct Settings {
    byte polygon_mod;
    byte user_input_dbg;
};

struct Camera {
    vec3 pos;
    vec3 front;
    vec3 up;
    float speed;
    float yaw;
    float pitch;
    float sensitivity;
    vec2 last_cursor_pos;
};

struct Context {
    struct Settings settings;
    struct Camera camera;
    byte keys[1024];
    float delta_time;
    float last_frame_time;
    GLint projection_loc;
    GLint view_loc;
    GLint model_loc;
    vec2 window_size;
    byte lock_mouse_mode;
};

void load_context(struct Context *ctx);

void upd_projection_mat(struct Context *ctx);
void upd_view_mat(struct Context *ctx);

void do_movement(struct Context *ctx);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
