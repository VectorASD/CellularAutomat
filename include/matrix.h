#pragma once

#include <vector.h>

typedef struct Matrix4 mat4;

struct Matrix4 {
    float a[4][4];
};

mat4 matrix4_new(float scale);
mat4 matrix4_copy(mat4 mat);

mat4 matrix4_mul(mat4 mat_a, mat4 mat_b);
vec4 matrix4_mul_v(mat4 mat, vec4 vec);
mat4 matrix4_mul_s(mat4 mat, float s);

void matrix4_repr(mat4 mat);

float ctg(float x);
float radians(float degrees);
float degrees(float radians);

mat4 perspective(float fovy, float aspect, float near, float far);
mat4 translate(mat4 in, vec3 pos);
mat4 rotate(mat4 in, float angle, vec3 norm_angles);
mat4 scale(mat4 in, vec3 scale);

void matrix4_test();
