#pragma once

#include <GL/glew.h>
#include <context.h>

void build_primitives_buffer(struct Context *ctx);

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void draw_triangle(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);
void draw_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

void render_primitives(struct Context *ctx);
