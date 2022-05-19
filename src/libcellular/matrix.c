#include <GL/glew.h> // GLint, GLfloat, glUniformMatrix4fv
#include <math.h>    // tan, M_PI, M_PI_2
#include <matrix.h>
#include <stdio.h>  // printf
#include <string.h> // memcpy

// Можно было бы местами использовать циклы, но здесь ценится только скорость из-за огромнейшей нагрузки именно на эти операции
// Хотел расписать строчки матриц по строкам программы, но увы, clang format в этом плане живёт своей жизнью = настройка подбирается автоматически... не правильно :/

mat4 matrix4_new(float scale) {
    mat4 mat = {{{scale, 0, 0, 0}, {0, scale, 0, 0}, {0, 0, scale, 0}, {0, 0, 0, 1}}};
    return mat;
}

mat4 matrix4_copy(mat4 mat) {
    mat4 res;
    memcpy(res.a, mat.a, sizeof(float) * 16);
    return res;
}

mat4 matrix4_mul(mat4 mat_a, mat4 mat_b) {
    mat4 res;
    float(*a)[4] = mat_a.a;
    float(*b)[4] = mat_b.a;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res.a[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];
    return res;
}

vec4 matrix4_mul_v(mat4 mat, vec4 vec) {
    float(*a)[4] = mat.a;
    vec4 res = {a[0][0] * vec.x + a[0][1] * vec.y + a[0][2] * vec.z + a[0][3] * vec.w, a[1][0] * vec.x + a[1][1] * vec.y + a[1][2] * vec.z + a[1][3] * vec.w, a[2][0] * vec.x + a[2][1] * vec.y + a[2][2] * vec.z + a[2][3] * vec.w, a[3][0] * vec.x + a[3][1] * vec.y + a[3][2] * vec.z + a[3][3] * vec.w};
    return res;
}

mat4 matrix4_mul_s(mat4 mat, float s) {
    float(*a)[4] = mat.a;
    mat4 res = {{{a[0][0] * s, a[0][1] * s, a[0][2] * s, a[0][3] * s}, {a[1][0] * s, a[1][1] * s, a[1][2] * s, a[1][3] * s}, {a[2][0] * s, a[2][1] * s, a[2][2] * s, a[2][3] * s}, {a[3][0] * s, a[3][1] * s, a[3][2] * s, a[3][3] * s}}};
    return res;
}

void matrix4_repr(mat4 mat) {
    float(*a)[4] = mat.a;
    printf("Matrix (4x4): [\n");
    for (int y = 0; y < 4; y++) {
        printf("  [");
        for (int x = 0; x < 4; x++) printf("%g, ", a[y][x]);
        printf("\b\b]\n");
    }
    printf("]\n");
}

float ctg(float x) {
    return tan(M_PI_2 - x);
}
float radians(float degrees) {
    return degrees * M_PI / 180;
}
float degrees(float radians) {
    return radians * 180 / M_PI;
}

mat4 perspective(float fovy, float aspect, float near, float far) {
    fovy = ctg(fovy / 2);
    mat4 mat = {{{fovy / aspect, 0, 0, 0}, {0, fovy, 0, 0}, {0, 0, -(far + near) / (far - near), -2 * far * near / (far - near)}, {0, 0, -1, 0}}};
    return mat;
}

mat4 translate(mat4 in, vec3 pos) {
    mat4 pos_mat = {{{1, 0, 0, pos.x}, {0, 1, 0, pos.y}, {0, 0, 1, pos.z}, {0, 0, 0, 1}}};
    return matrix4_mul(in, pos_mat);
}

mat4 rotate(mat4 in, float angle, vec3 norm_angles) {
    float rotX = angle * norm_angles.x;
    float rotY = angle * norm_angles.y;
    float rotZ = angle * norm_angles.z;
    if (rotY != 0) {
        float sin_r = sin(rotY), cos_r = cos(rotY);
        mat4 rot_mat = {{{cos_r, 0, sin_r, 0}, {0, 1, 0, 0}, {-sin_r, 0, cos_r, 0}, {0, 0, 0, 1}}};
        in = matrix4_mul(in, rot_mat);
    }
    if (rotX != 0) {
        float sin_r = sin(rotX), cos_r = cos(rotX);
        mat4 rot_mat = {{{1, 0, 0, 0}, {0, cos_r, -sin_r, 0}, {0, sin_r, cos_r, 0}, {0, 0, 0, 1}}};
        in = matrix4_mul(in, rot_mat);
    }
    if (rotZ != 0) {
        float sin_r = sin(rotZ), cos_r = cos(rotZ);
        mat4 rot_mat = {{{cos_r, -sin_r, 0, 0}, {sin_r, cos_r, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
        in = matrix4_mul(in, rot_mat);
    }
    return in;
}

mat4 scale(mat4 in, vec3 scale) {
    mat4 scale_mat = {{{scale.x, 0, 0, 0}, {0, scale.y, 0, 0}, {0, 0, scale.z, 0}, {0, 0, 0, 1}}};
    return matrix4_mul(in, scale_mat);
}

void matrix4_test() {
    matrix4_repr(perspective(radians(45), 4. / 3, 0.1, 100));

    mat4 trans = translate(matrix4_new(1), vector3_new(1, 1, 0));
    vector4_repr(matrix4_mul_v(trans, vector4_new(1, 0, 0, 1)));

    mat4 trans2 = rotate(matrix4_new(1), 90, vector3_new(0, 0, 1));
    matrix4_repr(scale(trans2, vector3_new(0.5, 0.5, 0.5)));

    vector3_repr(vector3_cross(vector3_new(2, -1, 3), vector3_new(5, 7, -4)));

    matrix4_repr(look_at(vector3_new(10, 2, 3), vector3_new(2, 3, 4), vector3_new(0, 1, 0)));
}

void matrix4_push(mat4 mat, GLint location) {
    float(*a)[4] = mat.a;
    const GLfloat value[] = {a[0][0], a[0][1], a[0][2], a[0][3], a[1][0], a[1][1], a[1][2], a[1][3], a[2][0], a[2][1], a[2][2], a[2][3], a[3][0], a[3][1], a[3][2], a[3][3]};
    glUniformMatrix4fv(location, 1, GL_TRUE, value);
}

const mat4 unit_mat = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

mat4 look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 z = vector3_norm(vector3_sub(eye, center));
    vec3 x = vector3_norm(vector3_cross(up, z));
    vec3 y = vector3_norm(vector3_cross(z, x));
    mat4 mat = {{{x.x, x.y, x.z, -vector3_dot(x, eye)}, {y.x, y.y, y.z, -vector3_dot(y, eye)}, {z.x, z.y, z.z, -vector3_dot(z, eye)}, {0, 0, 0, 1}}};
    return mat;
}
