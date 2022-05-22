#include <scenes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp

struct Model *create_model(text name, GLuint vertices_n, GLuint indices_n, GLfloat vertices[], GLuint indices[]) {
    struct Model *model = malloc(sizeof(struct Model));
    model->next = NULL;
    model->name = name;
    glGenBuffers(1, &model->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->EBO);

    glBindVertexArray(model->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_n, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_n, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return model;
}

void free_model(struct Model *model) {
    glDeleteBuffers(1, &model->VBO);
    glDeleteVertexArrays(1, &model->VAO);
    glDeleteBuffers(1, &model->EBO);
    free(model);
}

void add_model(struct Context *ctx, struct Model *model) {
    if (ctx->last_model == NULL)
        ctx->models = ctx->last_model = model;
    else
        ctx->last_model = ctx->last_model->next = model;
    ctx->models_n++;
}

void free_models(struct Context *ctx) {
    struct Model *p = ctx->models, *next;
    while (p) {
        next = p->next;
        free_model(p);
        p = next;
    }
}

void print_model_list(struct Context *ctx) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Доступные на данный момент модели:\n");
    struct Model *p = ctx->models;
    uint n = 0;
    while (p) {
        printf("  %2u: %s\n", n, p->name);
        n++;
        p = p->next;
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

struct Model *get_model_by_id(struct Context *ctx, uint id) {
    struct Model *p = ctx->models;
    uint n = 0;
    while (p) {
        if (id == n) return p;
        n++;
        p = p->next;
    }
    printf("Допустимое значение id модели от 0 до %u. Вы же запросили id: %u\n", n, id);
    print_model_list(ctx);
    exit(4);
}

struct Model *get_model_by_name(struct Context *ctx, text name) {
    struct Model *p = ctx->models;
    while (p) {
        if (strcmp(p->name, name) == 0) return p;
        p = p->next;
    }
    printf("Нет такой модели под именем: %s\n", name);
    print_model_list(ctx);
    exit(5);
}

void init_models(struct Context *ctx) {
    GLfloat vertices[] = {
        -1, -1, -1, 1, 0, 0,
        1, -1, -1, 0, 1, 0,
        -1, -1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0,
        -1, 1, -1, 0, 1, 1,
        1, 1, -1, 1, 0, 1,
        -1, 1, 1, 0, 1, 0.5,
        1, 1, 1, 1, 0.5, 0};
    GLuint indices[] = {
        0, 1, 2, 1, 2, 3,
        0, 1, 4, 1, 4, 5,
        0, 2, 4, 2, 4, 6,
        1, 3, 5, 3, 5, 7,
        2, 3, 6, 3, 6, 7,
        4, 5, 6, 5, 6, 7};
    add_model(ctx, create_model("куб", 6 * 8, 3 * 12, vertices, indices));
}
