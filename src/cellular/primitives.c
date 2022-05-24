#include <primitives.h>
#include <stdio.h>

void build_primitives_buffer(struct Context *ctx) {
    struct Primitives *prim = &ctx->prim;

    glGenBuffers(1, &prim->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);
    glGenVertexArrays(1, &prim->VAO);

    glBindVertexArray(prim->VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    printf("XD\n");
}

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    struct Primitives *prim = &ctx->prim;
    GLfloat vertices[] = {
        x1, y1, 0,
        x2, y2, 0};
    glBindVertexArray(prim->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
