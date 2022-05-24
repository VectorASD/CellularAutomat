#pragma once

#include <GL/glew.h>
#include <context.h>

void build_primitives_buffer(struct Context *ctx);
void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
