#pragma once

#include <GL/glew.h>
#include <context.h>

struct Model *create_model(text name, GLuint vertices_n, GLuint indices_n, GLfloat vertices[], GLuint indices[]);
void free_model(struct Model *model);

void add_model(struct Context *ctx, struct Model *model);
void free_models(struct Context *ctx);
void print_model_list(struct Context *ctx);
struct Model *get_model_by_id(struct Context *ctx, uint id);
struct Model *get_model_by_name(struct Context *ctx, text name);
void init_models(struct Context *ctx);
