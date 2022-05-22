#include <math.h>  // powf
#include <stdio.h> // printf
#include <vector.h>

//
// Vector2 functions
//

vec2 vector2_new(float x, float y) {
    vec2 res = {x, y};
    return res;
}

vec2 vector2_add(vec2 a, vec2 b) {
    vec2 res = {a.x + b.x, a.y + b.y};
    return res;
}

vec2 vector2_add_s(vec2 a, float scale) {
    vec2 res = {a.x + scale, a.y + scale};
    return res;
}

vec2 vector2_sub(vec2 a, vec2 b) {
    vec2 res = {a.x - b.x, a.y - b.y};
    return res;
}

vec2 vector2_sub_s(vec2 a, float scale) {
    vec3 res = {a.x - scale, a.y - scale};
    return res;
}

vec2 vector2_mul(vec2 a, vec2 b) {
    vec2 res = {a.x * b.x, a.y * b.y};
    return res;
}

vec2 vector2_mul_s(vec2 a, float scale) {
    vec2 res = {a.x * scale, a.y * scale};
    return res;
}

vec2 vector2_div(vec2 a, vec2 b) {
    vec2 res = {a.x / b.x, a.y / b.y};
    return res;
}

vec2 vector2_div_s(vec2 a, float scale) {
    vec3 res = {a.x / scale, a.y / scale};
    return res;
}

float vector2_len(vec2 vec) {
    return powf(vec.x * vec.x + vec.y * vec.y, 0.5);
}

vec2 vector2_norm(vec2 vec) {
    float len = vector2_len(vec);
    if (len == 0) return vec;
    return vector2_div_s(vec, len);
}

void vector2_repr(vec2 vec) {
    printf("Vector2: (%g; %g)\n", vec.x, vec.y);
}

float vector2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

//
// Vector3 functions
//

vec3 vector3_new(float x, float y, float z) {
    vec3 res = {x, y, z};
    return res;
}

vec3 vector3_add(vec3 a, vec3 b) {
    vec3 res = {a.x + b.x, a.y + b.y, a.z + b.z};
    return res;
}

vec3 vector3_add_s(vec3 a, float scale) {
    vec3 res = {a.x + scale, a.y + scale, a.z + scale};
    return res;
}

vec3 vector3_sub(vec3 a, vec3 b) {
    vec3 res = {a.x - b.x, a.y - b.y, a.z - b.z};
    return res;
}

vec3 vector3_sub_s(vec3 a, float scale) {
    vec3 res = {a.x - scale, a.y - scale, a.z - scale};
    return res;
}

vec3 vector3_mul(vec3 a, vec3 b) {
    vec3 res = {a.x * b.x, a.y * b.y, a.z * b.z};
    return res;
}

vec3 vector3_mul_s(vec3 a, float scale) {
    vec3 res = {a.x * scale, a.y * scale, a.z * scale};
    return res;
}

vec3 vector3_div(vec3 a, vec3 b) {
    vec3 res = {a.x / b.x, a.y / b.y, a.z / b.z};
    return res;
}

vec3 vector3_div_s(vec3 a, float scale) {
    vec3 res = {a.x / scale, a.y / scale, a.z / scale};
    return res;
}

float vector3_len(vec3 vec) {
    return powf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z, 0.5);
}

vec3 vector3_norm(vec3 vec) {
    float len = vector3_len(vec);
    if (len == 0) return vec;
    return vector3_div_s(vec, len);
}

void vector3_repr(vec3 vec) {
    printf("Vector3: (%g; %g; %g)\n", vec.x, vec.y, vec.z);
}

vec3 vector3_cross(vec3 a, vec3 b) {
    vec3 res = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return res;
}

float vector3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//
// Vector4 functions
//

vec4 vector4_new(float x, float y, float z, float w) {
    vec4 res = {x, y, z, w};
    return res;
}

vec4 vector4_add(vec4 a, vec4 b) {
    vec4 res = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return res;
}

vec4 vector4_add_s(vec4 a, float scale) {
    vec4 res = {a.x + scale, a.y + scale, a.z + scale, a.w + scale};
    return res;
}

vec4 vector4_sub(vec4 a, vec4 b) {
    vec4 res = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return res;
}

vec4 vector4_sub_s(vec4 a, float scale) {
    vec4 res = {a.x - scale, a.y - scale, a.z - scale, a.w - scale};
    return res;
}

vec4 vector4_mul(vec4 a, vec4 b) {
    vec4 res = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return res;
}

vec4 vector4_mul_s(vec4 a, float scale) {
    vec4 res = {a.x * scale, a.y * scale, a.z * scale, a.w * scale};
    return res;
}

vec4 vector4_div(vec4 a, vec4 b) {
    vec4 res = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
    return res;
}

vec4 vector4_div_s(vec4 a, float scale) {
    vec4 res = {a.x / scale, a.y / scale, a.z / scale, a.w / scale};
    return res;
}

float vector4_len(vec4 vec) {
    return powf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w, 0.5);
}

vec4 vector4_norm(vec4 vec) {
    float len = vector4_len(vec);
    if (len == 0) return vec;
    return vector4_div_s(vec, len);
}

void vector4_repr(vec4 vec) {
    printf("Vector4: (%g; %g; %g; %g)\n", vec.x, vec.y, vec.z, vec.w);
}

float vector4_dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
