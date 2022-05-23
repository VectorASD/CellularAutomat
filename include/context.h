#pragma once

#include <matrix.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
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

struct Model {
    struct Model *next;
    GLuint VBO; //Vertex Buffer Object
    GLuint VAO; //Vertex Array Object
    GLuint EBO; //Elements Buffer Object
    text name;
    GLuint indices;
};

struct Part {
    struct Part *next, *pred;
    struct Scene *scene;
    struct Model *used_model;
    mat4 model_mat;
    vec3 pos;
    vec3 orientation;
    vec3 size;
};

struct Scene {
    struct Scene *next;
    struct Part *parts;
    struct Part *last_part;
    int parts_n;
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
    struct Scene *scenes;
    struct Scene *last_scene;
    struct Scene *current_scene;
    int scenes_n;
    struct Model *models;
    struct Model *last_model;
    int models_n;
};

void load_context(struct Context *ctx);

void upd_projection_mat(struct Context *ctx);
void upd_view_mat(struct Context *ctx);

void do_movement(struct Context *ctx);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void window_size_callback(GLFWwindow* window, int width, int height);

int randint(int a, int b);
