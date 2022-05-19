#pragma once

#include <vector.h>

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
};

struct Context {
    struct Settings settings;
    struct Camera camera;
    byte keys[1024];
    float delta_time;
    float last_frame_time;
};

void load_context(struct Context *ctx);
void do_movement(struct Context *ctx);
