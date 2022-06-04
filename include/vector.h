#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
typedef const char *text;

typedef struct Vector2 vec2;
typedef struct Vector3 vec3;
typedef struct Vector4 vec4;

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};



vec2 vector2_new(float x, float y);
byte vector2_cmp(vec2 a, vec2 b);

vec2 vector2_add(vec2 a, vec2 b);
vec2 vector2_add_s(vec2 a, float scale);

vec2 vector2_sub(vec2 a, vec2 b);
vec2 vector2_sub_s(vec2 a, float scale);

vec2 vector2_mul(vec2 a, vec2 b);
vec2 vector2_mul_s(vec2 a, float scale);

vec2 vector2_div(vec2 a, vec2 b);
vec2 vector2_div_s(vec2 a, float scale);

float vector2_len(vec2 vec);
vec2 vector2_norm(vec2 vec);
void vector2_repr(vec2 vec);

float vector2_dot(vec2 a, vec2 b);



vec3 vector3_new(float x, float y, float z);
byte vector3_cmp(vec3 a, vec3 b);

vec3 vector3_add(vec3 a, vec3 b);
vec3 vector3_add_s(vec3 a, float scale);

vec3 vector3_sub(vec3 a, vec3 b);
vec3 vector3_sub_s(vec3 a, float scale);

vec3 vector3_mul(vec3 a, vec3 b);
vec3 vector3_mul_s(vec3 a, float scale);

vec3 vector3_div(vec3 a, vec3 b);
vec3 vector3_div_s(vec3 a, float scale);

float vector3_len(vec3 vec);
vec3 vector3_norm(vec3 vec);
void vector3_repr(vec3 vec);

vec3 vector3_cross(vec3 a, vec3 b);
float vector3_dot(vec3 a, vec3 b);



vec4 vector4_new(float x, float y, float z, float w);
byte vector4_cmp(vec4 a, vec4 b);

vec4 vector4_add(vec4 a, vec4 b);
vec4 vector4_add_s(vec4 a, float scale);

vec4 vector4_sub(vec4 a, vec4 b);
vec4 vector4_sub_s(vec4 a, float scale);

vec4 vector4_mul(vec4 a, vec4 b);
vec4 vector4_mul_s(vec4 a, float scale);

vec4 vector4_div(vec4 a, vec4 b);
vec4 vector4_div_s(vec4 a, float scale);

float vector4_len(vec4 vec);
vec4 vector4_norm(vec4 vec);
void vector4_repr(vec4 vec);

float vector4_dot(vec4 a, vec4 b);
