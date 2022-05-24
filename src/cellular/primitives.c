#include <primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy

void build_primitives_buffer(struct Context *ctx) {
    struct Primitives *prim = &ctx->prim;

    glGenBuffers(1, &prim->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);
    glGenVertexArrays(1, &prim->VAO);

    glBindVertexArray(prim->VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    prim->lines.first = NULL;
    prim->lines.last = NULL;
    prim->lines.n = 0;
    prim->triangles.first = NULL;
    prim->triangles.last = NULL;
    prim->triangles.n = 0;
    prim->line_color = vector4_new(1, 1, 0, 1);
    prim->line_color2 = vector4_new(0, 0, 1, 1);
    prim->tri_color = vector4_new(0, 1, 0, 1);
    prim->tri_color2 = vector4_new(0, 0, 1, 1);
    prim->tri_color3 = vector4_new(1, 1, 0, 1);
    prim->tri_color4 = vector4_new(1, 0.5, 0, 1);
}

void add_vertex_node(GLfloat aspect, struct VertexList *list, GLfloat x, GLfloat y, GLfloat z, vec4 *color) {
    x = x * aspect / 600 * 2 - 1;
    y = 1 - y / 600 * 2;
    struct VertexNode orig = {NULL, x, y, z, color->x, color->y, color->z, color->w};
    if (list->first == NULL) {
        struct VertexNode *vertex = malloc(sizeof(struct VertexNode));
        memcpy(vertex, &orig, sizeof(struct VertexNode));
        list->first = list->last = vertex;
    } else if (list->last->next == NULL) {
        struct VertexNode *vertex = malloc(sizeof(struct VertexNode));
        memcpy(vertex, &orig, sizeof(struct VertexNode));
        list->last = list->last->next = vertex;
    } else {
        if (list->n) list->last = list->last->next;
        memcpy(&list->last->x, &orig.x, 7 * sizeof(GLfloat));
    }
    list->n++;
}

void nodes_to_array(struct VertexList *list, GLfloat *arr) {
    struct VertexNode *p = list->first;
    int pos = 0;
    for (int i = 0; i < list->n; i++) {
        memcpy(arr + pos, &p->x, 7 * sizeof(GLfloat));
        pos += 7;
        p = p->next;
    }
    list->last = list->first;
    list->n = 0;
}

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *lines = &prim->lines;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, lines, x1, y1, -1, &prim->line_color);
    add_vertex_node(aspect, lines, x2, y2, -1, &prim->line_color2);
}

void draw_triangle(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, triangles, x1, y1, -1, &prim->tri_color);
    add_vertex_node(aspect, triangles, x2, y2, -1, &prim->tri_color2);
    add_vertex_node(aspect, triangles, x3, y3, -1, &prim->tri_color3);
}

void draw_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GLfloat x2 = x + width, y2 = y + height;
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, triangles, x, y, -1, &prim->tri_color);
    add_vertex_node(aspect, triangles, x2, y, -1, &prim->tri_color2);
    add_vertex_node(aspect, triangles, x2, y2, -1, &prim->tri_color4);
    add_vertex_node(aspect, triangles, x, y, -1, &prim->tri_color);
    add_vertex_node(aspect, triangles, x, y2, -1, &prim->tri_color3);
    add_vertex_node(aspect, triangles, x2, y2, -1, &prim->tri_color4);
}

void render_primitives(struct Context *ctx) {
    struct Primitives *prim = &ctx->prim;
    int lines_n = prim->lines.n;
    int triangles_n = prim->triangles.n;
    GLfloat vertices[7 * (lines_n > triangles_n ? lines_n : triangles_n)];

    glBindVertexArray(prim->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);

    nodes_to_array(&prim->lines, vertices);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * lines_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, lines_n);

    nodes_to_array(&prim->triangles, vertices);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * triangles_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, triangles_n);

    glBindVertexArray(0);
}
