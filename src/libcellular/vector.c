#include <math.h>  // powf
#include <stdio.h> // printf
#include <vector.h>

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
    return vector3_div_s(vec, vector3_len(vec));
}

vec3 vector3_cross(vec3 a, vec3 b) {
    vec3 res = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return res;
}

void vector3_repr(vec3 vec) {
    printf("Vector3: (%g; %g; %g)\n", vec.x, vec.y, vec.z);
}

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
    return vector4_div_s(vec, vector4_len(vec));
}

void vector4_repr(vec4 vec) {
    printf("Vector4: (%g; %g; %g; %g)\n", vec.x, vec.y, vec.z, vec.w);
}
