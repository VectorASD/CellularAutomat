#pragma once

#include <GL/glew.h>
#include <context.h>
#include <primitives.h>

struct Model *create_model(text name, GLuint vertices_n, GLuint indices_n, GLfloat vertices[], GLuint indices[]);
void free_model(struct Model *model);

void add_model(struct Context *ctx, struct Model *model);
void print_model_list(struct Context *ctx);
struct Model *get_model_by_id(struct Context *ctx, uint id);
struct Model *get_model_by_name(struct Context *ctx, text name);
void init_models(struct Context *ctx);

struct Part *create_part(struct Context *ctx, struct Model *model);
void update_part(struct Part *part);
void delete_part(struct Part *part);

void select_scene(struct Context *ctx, uint id);
void render_scene(struct Context *ctx);
void free_scenes(struct Context *ctx);
uint bind_scene(struct Context *ctx, text name, void (*init)(struct Scene *scene), void (*render)(struct Scene *scene), void (*local_gui)(struct Scene *scene), void (*free)(struct Scene *scene));
