#pragma once

typedef struct Vector3 vec3;
typedef struct Vector4 vec4;

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};



vec3 vector3_new(float x, float y, float z);

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
