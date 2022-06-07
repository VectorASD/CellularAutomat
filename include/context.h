#pragma once

#include <ft2build.h>
#include <matrix.h>
#include FT_FREETYPE_H

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
    vec4 color;
    vec4 edge_color;
    byte color_mode;
    byte visible;
};

struct Scene {
    struct Scene *next;
    struct Part *parts;
    struct Part *last_part;
    int parts_n;
    void *user_pointer;
    struct Context *ctx;
    void (*init)(struct Scene *scene);
    void (*render)(struct Scene *scene);
    void (*local_gui)(struct Scene *scene);
    void (*free)(struct Scene *scene);
    byte first_tick;
    text name;
};

struct VertexNode {
    struct VertexNode *next;
    GLfloat x, y, z, r, g, b, a;
    short btn_id;
    void (*btn_callback)(struct Scene *scene, byte button);
    void *btn_attr;
    struct Character *glyph;
    GLfloat glyph_height;
};

struct VertexList {
    struct VertexNode *first;
    struct VertexNode *last;
    ushort n;
    ushort dropped_n;
};

struct Primitives {
    GLuint VBO;
    GLuint VAO;
    struct VertexList lines;
    struct VertexList triangles;
    vec4 line_color;
    vec4 line_color2;
    vec4 tri_color;
    vec4 tri_color2;
    vec4 tri_color3;
    vec4 tri_color4;
    int all_prims_n;
    short btn_id;
    void *btn_attr;
    short buttons;
    void (*btn_callback)(struct Scene *scene, byte button);
    struct Character *glyph;
    GLfloat glyph_height;
};

struct Character {
    GLuint texture_id;
    vec2 size;
    vec2 bearing;
    GLuint advance;
    uint code;
};

struct CharNode {
    struct CharNode *left, *right;
    struct Character chr;
    uint code;
    ushort height;
};

struct Font {
    FT_Library ft; // в freetype/freetype.h на 1072 строчке описана эта структура
    FT_Face face;
    struct CharNode *map;
    uint current_height;
    GLuint font_program;
    GLint color_loc;
    vec4 text_color;
    GLuint VBO;
    GLuint VAO;
    byte align_left;
    byte align_up;
    GLfloat width_limit;
    struct VertexList glyphs;
};

struct Menus {
    byte show_menu;
    byte show_menu2;
};

struct Context {
    struct Settings settings;
    struct Camera camera;
    struct Primitives prim;
    struct Font font;
    struct Menus menus;
    
    GLFWwindow *window;
    float time;
    float time_delta;
    float last_frame_time;
    char fps_view[6][64];
    byte fps_view_n;
    
    byte keys[1024];
    vec4 window_size;
    GLint window_center[2];
    byte mouse_lock_mode;
    short btn_id_press[8];
    short btn_id_release[8];
    byte btn_mouse_pressed[8];
    byte btn_mouse_clicked[8];
    byte mouse_pressed;
    byte lock_mouse;
    
    struct Scene *scenes;
    struct Scene *last_scene;
    struct Scene *current_scene;
    int scenes_n;
    struct Model *models;
    struct Model *last_model;
    int models_n;
    GLuint ssbo;
    struct Part *hovered_part;
    void *btn_attr;
    
    GLint projection_loc;
    GLint view_loc;
    GLint model_loc;
    GLuint gui_program;
    GLuint shader_program;
    GLint main_color_loc;
    GLint edge_color_loc;
    GLint color_mode_loc;
    GLint cursor_pos_loc;
    GLint part_id_loc;
};

void load_context(struct Context *ctx);

void upd_projection_mat(struct Context *ctx);
void upd_view_mat(struct Context *ctx);

void context_tick(struct Context *ctx);

int randint(int a, int b);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void window_size_callback(GLFWwindow *window, int width, int height);



float min(float a, float b);
float max(float a, float b);
float line_dist(float x, float y, float x2, float y2);
float dot_to_line_dist(float x, float y, float x2, float y2, float dot_x, float dot_y);
short calculate_hovered_button(struct Context *ctx, byte use_dropped_n);
