#pragma once

#include <GL/glew.h>
#include <context.h>

void init_primitives(struct Context *ctx);

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void draw_triangle(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);
void draw_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
void draw_rect(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
void draw_rect_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

void set_line_color(struct Context *ctx, float R, float G, float B, float A);
void set_line_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A);
void set_box_color(struct Context *ctx, float R, float G, float B, float A);
void set_box_vert_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A);
void set_box_horiz_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A);

void render_primitives(struct Context *ctx);
void free_primitives(struct Context *ctx);



void init_fonts(struct Context *ctx);
void render_glyphs(struct Context *ctx);
void free_fonts(struct Context *ctx);

void set_font_height(struct Context *ctx, uint height);
void set_text_color(struct Context *ctx, float R, float G, float B, float A);
vec4 measure_text(struct Context *ctx, text str);
void set_text_alignment(struct Context *ctx, byte align_left, byte align_up, GLfloat width_limit, GLfloat height_limit);
void render_text(struct Context *ctx, text str, GLfloat x, GLfloat y, GLfloat scale);



void draw_btn_rect(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
void draw_button(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height, void (*btn_callback)(struct Scene *scene, byte button));
void set_button_color(struct Context *ctx, float box_R, float box_G, float box_B, float line_R, float line_G, float line_B, float A);
